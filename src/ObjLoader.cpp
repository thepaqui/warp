/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjLoader.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:22:20 by Copilot           #+#    #+#             */
/*   Updated: 2026/02/13 19:19:57 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ObjLoader.hpp"

// Key for unique vertex (position/texcoord/normal/smoothing group/face) to avoid duplication
struct VertexKey {
	int vIdx;
	int vtIdx;
	int vnIdx;
	int smoothingGroup;
	int faceId;
	bool operator==(const VertexKey &o) const {
		return (
			vIdx == o.vIdx
			&& vtIdx == o.vtIdx
			&& vnIdx == o.vnIdx
			&& smoothingGroup == o.smoothingGroup
			&& faceId == o.faceId
		);
	}
};

// Hash function for VertexKey to be used in unordered_map
// ^= is the bitwise XOR assignment operator
// + 0x9e3779b9 + (h<<6) + (h>>2) is the boost hash combine method
// which helps give better distribution of hash values
// which is important for performance in hash tables
struct VertexKeyHasher {
	size_t operator()(const VertexKey &k) const {
		size_t seed = std::hash<int>()(k.vIdx);
		seed ^= std::hash<int>()(k.vtIdx) + 0x9e3779b97f4a7c15ULL + (seed<<6) + (seed>>2);
		seed ^= std::hash<int>()(k.vnIdx) + 0x9e3779b97f4a7c15ULL + (seed<<6) + (seed>>2);
		seed ^= std::hash<int>()(k.smoothingGroup) + 0x9e3779b97f4a7c15ULL + (seed<<6) + (seed>>2);
		seed ^= std::hash<int>()(k.faceId) + 0x9e3779b97f4a7c15ULL + (seed<<6) + (seed>>2);
		return seed;
	}
};

static std::string getBaseDir(const std::string &path)
{
	auto sep = path.find_last_of("/");
	return (sep == std::string::npos) ? std::string(".") : path.substr(0, sep);
}

// Per-part data
struct PartData {
	ObjMeshData mesh;
	std::unordered_map<VertexKey, GLuint, VertexKeyHasher> vertexMap; // Maps unique vertex keys to indices
	std::vector<int> vertexToPosIndex; // Maps vertex index in mesh to position index for normal accumulation
	std::vector<int> vertexToSmoothGroup; // Maps vertex index in mesh to smoothing group for normal accumulation
	std::map<int, std::vector<MATF>> accumNormalsByGroup; // Accumulated normals for each position index by smoothing group
};

static std::string makePartKey(const std::string &group, const std::string &material)
{
	return group + "|" + material;
}

static void parse_mtllib_tag
(
	std::istringstream& ls,
	const std::string &baseDir,
	std::map<std::string, Material> &materials
)
{
	std::string mtlname;
	ls >> mtlname;
	if (mtlname.empty())
		return;
	std::string mtlPath = baseDir + "/" + mtlname;
	parseMTL(mtlPath, materials);
}

static void parse_g_tag
(
	std::istringstream& ls,
	std::string &currentGroup
)
{
	std::string gname;
	ls >> gname;
	if (gname.empty())
		gname = "default";
	currentGroup = gname;
}

static void parse_usemtl_tag
(
	std::istringstream& ls,
	std::string &currentMaterial
)
{
	ls >> currentMaterial;
}

static void parse_s_tag
(
	std::istringstream& ls,
	int &currentSmoothingGroup
)
{
	std::string svalue;
	ls >> svalue;
	if (svalue.empty())
		return;
	if (svalue == "off" || svalue == "0")
		currentSmoothingGroup = 0;
	else if (svalue == "on")
		currentSmoothingGroup = 1;
	else
		currentSmoothingGroup = std::stoi(svalue);
}

static void parse_v_tag
(
	std::istringstream& ls,
	std::vector<MATF> &positions,
	float &minY,
	float &maxY
)
{
	float x,y,z;
	if (!(ls >> x >> y >> z))
		throw std::runtime_error("ObjLoader::loadModel: malformed v line");

	positions.push_back(TVEC3(x,y,z));

	if (y < minY)
		minY = y;
	if (y > maxY)
		maxY = y;
}

static void parse_vt_tag
(
	std::istringstream& ls,
	std::vector<MATF> &texcoords
)
{
	float u, v;
	if (!(ls >> u >> v))
		throw std::runtime_error("ObjLoader::loadModel: malformed vt line");

	texcoords.push_back(TVEC3(u, v, 0.f));
}

static void parse_vn_tag
(
	std::istringstream& ls,
	std::vector<MATF> &normals
)
{
	float x, y, z;
	if (!(ls >> x >> y >> z))
		throw std::runtime_error("ObjLoader::loadModel: malformed vn line");

	normals.push_back(TVEC3(x, y, z));
}

static const int MISSING_IDX = std::numeric_limits<int>::min();

// Parses the indices from a vertSpec of the form `v/vt/vn`, `v//vn`, `v/vt`, or `v`
static void parse_f_tag_vertSpec_indices
(
	std::string& vertSpec,
	int &vi,
	int &vti,
	int &vni
)
{
	auto slash1 = vertSpec.find('/');
	if (slash1 == std::string::npos) {
		// `v` format
		// Get vi (position index)
		vi = std::stoi(vertSpec);
	} else {
		// Get vi (position index)
		std::string s_vi = vertSpec.substr(0, slash1);
		vi = s_vi.empty() ? MISSING_IDX : std::stoi(s_vi);

		auto slash2 = vertSpec.find('/', slash1 + 1);
		if (slash2 == std::string::npos) {
			// `v/vt` format
			// Get vti (texcoord index)
			std::string s_vt = vertSpec.substr(slash1 + 1);
			vti = s_vt.empty() ? MISSING_IDX : std::stoi(s_vt);
		} else {
			// `v/vt/vn` or `v//vn` formats
			// Get vti (texcoord index)
			std::string s_vt = vertSpec.substr(slash1 + 1, slash2 - (slash1 + 1));
			vti = s_vt.empty() ? MISSING_IDX : std::stoi(s_vt);

			// Get vni (normal index)
			std::string s_vn = vertSpec.substr(slash2 + 1);
			vni = s_vn.empty() ? MISSING_IDX : std::stoi(s_vn);
		}
	}
}

// Fixes an index based on element count and whether 0-based indices are used
// Returns -1 if any error is detected
static int parse_f_tag_vertSpec_fixIndex
(
	int idx,
	int count,
	bool zeroBased
)
{
	// Missing index or no elements
	if (count == 0 || idx == MISSING_IDX)
		return -1;

	// Convert to 0-based if 1-based and positive
	if (!zeroBased && idx > 0)
		idx--;

	// Handle negative indices
	if (idx < 0)
		idx = count + idx;

	// Detect out of bounds
	if (idx < 0 || idx >= count)
		return -1;

	return idx;
}

// Parses a vertSpec of the form `v/vt/vn`, `v//vn`, `v/vt`, or `v`
// and appends the fixed indices to the face vector
static void parse_f_tag_vertSpec
(
	std::vector<std::tuple<int,int,int>> &face,
	std::string& vertSpec,
	std::vector<MATF> &positions,
	std::vector<MATF> &texcoords,
	std::vector<MATF> &normals
)
{
	int vi = MISSING_IDX;  // position index
	int vti = MISSING_IDX; // texcoord index
	int vni = MISSING_IDX; // normal index

	bool zeroBased = false; // Detect 0-based indices (non-standard)

	parse_f_tag_vertSpec_indices(vertSpec, vi, vti, vni);
	if (vi == 0 || vti == 0 || vni == 0)
		zeroBased = true;
	int fixedVi = parse_f_tag_vertSpec_fixIndex(vi, (int)positions.size(), zeroBased);
	int fixedVti = parse_f_tag_vertSpec_fixIndex(vti, (int)texcoords.size(), zeroBased);
	int fixedVni = parse_f_tag_vertSpec_fixIndex(vni, (int)normals.size(), zeroBased);
	if (fixedVi < 0)
		throw std::runtime_error("ObjLoader::loadModel: invalid position index in face");
	face.emplace_back(fixedVi, fixedVti, fixedVni);
}

// Adds a vertex to the part's mesh if not already present, returns its index
static GLuint addVertexPart
(
	PartData& part,
	std::vector<MATF> &positions,
	std::vector<MATF> &texcoords,
	std::vector<MATF> &normals,
	int vi,
	int vti,
	int vni,
	int smoothingGroup,
	int faceId,
	const MATF *overrideNormal
)
{
	if (vi < 0) // double check but better safe than sorry
		throw std::runtime_error("ObjLoader::loadModel: invalid position index in face");

	VertexKey key{vi, vti, vni, smoothingGroup, faceId};
	auto it = part.vertexMap.find(key);
	if (it != part.vertexMap.end())
		return it->second; // Vertex already exists

	Vertex vert;
	// Position
	const MATF &pos = positions.at(vi);
	vert.position[0] = pos.getElem(0);
	vert.position[1] = pos.getElem(1);
	vert.position[2] = pos.getElem(2);

	// Normal
	if (vni >= 0) {
		const MATF &norm = normals.at(vni);
		vert.normal[0] = norm.getElem(0);
		vert.normal[1] = norm.getElem(1);
		vert.normal[2] = norm.getElem(2);
	} else if (overrideNormal) {
		vert.normal[0] = overrideNormal->getElem(0);
		vert.normal[1] = overrideNormal->getElem(1);
		vert.normal[2] = overrideNormal->getElem(2);
	} else {
		vert.normal[0] = 0.f;
		vert.normal[1] = 0.f;
		vert.normal[2] = 1.f;
	}

	// TexCoords
	if (vti >= 0) {
		const MATF &tc = texcoords.at(vti);
		vert.texCoords[0] = tc.getElem(0);
		vert.texCoords[1] = tc.getElem(1);
	} else {
		vert.texCoords[0] = 0.f;
		vert.texCoords[1] = 0.f;
	}

	part.mesh.vertices.push_back(vert);
	GLuint idx = static_cast<GLuint>(part.mesh.vertices.size() - 1);
	part.vertexMap.emplace(key, idx);
	return idx;
}

// Resizes the accumulator if necessary
static void resizeAccums(size_t posCount, std::vector<MATF> &accum)
{
	if (accum.size() < posCount)
		accum.resize(posCount, TVEC3(0.f, 0.f, 0.f));
}

// adds the face normal to the accumulator for later smoothing
static void parse_f_tag_accumulate_face_normal
(
	PartData &pd,
	const std::vector<MATF> &positions,
	int smoothingGroup,
	int p0Idx,
	int p1Idx,
	int p2Idx
)
{
	if (smoothingGroup <= 0)
		return;
	auto &accum = pd.accumNormalsByGroup[smoothingGroup];
	resizeAccums(positions.size(), accum);
	const MATF &p0 = positions[p0Idx];
	const MATF &p1 = positions[p1Idx];
	const MATF &p2 = positions[p2Idx];
	MATF u = p1 - p0;
	MATF v = p2 - p0;
	MATF normal = MATF::normalize(MATF::cross(u, v));
	accum[p0Idx] = accum[p0Idx] + normal;
	accum[p1Idx] = accum[p1Idx] + normal;
	accum[p2Idx] = accum[p2Idx] + normal;
}

static void parse_f_tag_fan_triangulate_and_generate_normals
(
	std::map<std::string, PartData> &parts,
	std::vector<MATF> &positions,
	std::vector<MATF> &texcoords,
	std::vector<MATF> &normals,
	const std::string &currentGroup,
	const std::string &currentMaterial,
	const std::vector<std::tuple<int,int,int>> &face,
	int smoothingGroup,
	size_t &faceIdCounter
)
{
	std::string	partKey = makePartKey(currentGroup, currentMaterial);
	PartData&	pd = parts[partKey];
	const bool	generateNormals = normals.empty();
	const bool	smoothingOff = generateNormals && (smoothingGroup <= 0);
	const int	smooth = generateNormals ? smoothingGroup : 0; // the smoothing group or 0 if normals are already provided, since the normals already differentiate vertices
	int			faceId = smoothingOff ? static_cast<int>(faceIdCounter++) : -1; // separate faces by id for sharp edges when smoothing is off, this will allow duplicate vertices with different normals for a "low-poly" effect

	MATF		faceNormal = TVEC3(0.f, 0.f, 1.f);
	// basic face normal, only defined when smoothing is turned off
	const MATF	*faceNormalPtr = nullptr;
	if (smoothingOff) {
		const MATF &p0 = positions[std::get<0>(face[0])];
		const MATF &p1 = positions[std::get<0>(face[1])];
		const MATF &p2 = positions[std::get<0>(face[2])];
		MATF u = p1 - p0;
		MATF v = p2 - p0;
		faceNormal = MATF::normalize(MATF::cross(u, v));
		faceNormalPtr = &faceNormal;
	}

	GLuint i0 = addVertexPart
	(
		pd, positions, texcoords, normals,
		std::get<0>(face[0]),
		std::get<1>(face[0]),
		std::get<2>(face[0]),
		smooth, faceId, faceNormalPtr
	);
	// fan triangulation
	for (size_t i = 1; i + 1 < face.size(); ++i)
	{
		GLuint i1 = addVertexPart
		(
			pd, positions, texcoords, normals,
			std::get<0>(face[i]),
			std::get<1>(face[i]),
			std::get<2>(face[i]),
			smooth, faceId, faceNormalPtr
		);
		GLuint i2 = addVertexPart
		(
			pd, positions, texcoords, normals,
			std::get<0>(face[i + 1]),
			std::get<1>(face[i + 1]),
			std::get<2>(face[i + 1]),
			smooth, faceId, faceNormalPtr
		);
		pd.mesh.indices.push_back(i0);
		pd.mesh.indices.push_back(i1);
		pd.mesh.indices.push_back(i2);

		if (generateNormals && !smoothingOff) {
			int p0Idx = std::get<0>(face[0]);
			int p1Idx = std::get<0>(face[i]);
			int p2Idx = std::get<0>(face[i + 1]);
			parse_f_tag_accumulate_face_normal(pd, positions, smoothingGroup, p0Idx, p1Idx, p2Idx);
		}
	}
}

static void parse_f_tag
(
	std::istringstream& ls,
	std::map<std::string, PartData> &parts,
	std::vector<MATF> &positions,
	std::vector<MATF> &texcoords,
	std::vector<MATF> &normals,
	const std::string &currentGroup,
	const std::string &currentMaterial,
	int smoothingGroup,
	size_t &faceIdCounter
)
{
	std::vector<std::tuple<int,int,int>> face;
	std::string vertSpec;
	while (ls >> vertSpec)
		parse_f_tag_vertSpec(face, vertSpec, positions, texcoords, normals);

	if (face.size() < 3)
		throw std::runtime_error("ObjLoader::loadModel: face with less than 3 vertices");

	parse_f_tag_fan_triangulate_and_generate_normals(
		parts, positions, texcoords, normals,
		currentGroup, currentMaterial, face,
		smoothingGroup, faceIdCounter
	);
}

ObjModelData ObjLoader::loadModel(const std::string &path)
{
	std::ifstream in(path);
	if (!in.good())
		throw std::runtime_error("ObjLoader::loadModel: cannot open file: " + path);

	std::vector<MATF> positions;
	std::vector<MATF> texcoords; // stored as vec3(u, v, 0)
	std::vector<MATF> normals;

	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();

	std::map<std::string, PartData> parts; // key = groupName + '|' + materialName
	std::map<std::string, Material> materials;

	std::string currentGroup = "default";
	std::string currentMaterial;
	int currentSmoothingGroup = 0; // Off by default
	size_t faceIdCounter = 0; // counter for face ids when smoothing is off, see parse_f_tag_fan_triangulate_and_generate_normals

	auto &inStream = in;
	std::string line;
	// store dir of the OBJ file to resolve relative mtllib paths
	const std::string baseDir = getBaseDir(path);

	while (std::getline(inStream, line)) {
		size_t start = line.find_first_not_of(" \t\r");
		if (start == std::string::npos)
			continue; // empty line
		if (line[start] == '#')
			continue; // comment

		std::istringstream ls(line);
		std::string tag;
		ls >> tag;
		if (tag == "mtllib") {
			parse_mtllib_tag(ls, baseDir, materials);
		} else if (tag == "g") {
			parse_g_tag(ls, currentGroup);
		} else if (tag == "usemtl") {
			parse_usemtl_tag(ls, currentMaterial);
		} else if (tag == "s") {
			parse_s_tag(ls, currentSmoothingGroup);
		} else if (tag == "v") {
			parse_v_tag(ls, positions, minY, maxY);
		} else if (tag == "vt") {
			parse_vt_tag(ls, texcoords);
		} else if (tag == "vn") {
			parse_vn_tag(ls, normals);
		} else if (tag == "f") {
			parse_f_tag
			(
				ls, parts,
				positions, texcoords, normals,
				currentGroup, currentMaterial,
				currentSmoothingGroup, faceIdCounter
			);
		}
	}

	// Build ObjModelData from parts
	ObjModelData model;
	model.materials = std::move(materials);
	std::string actualDefMatName = ""; // This is only used if some parts have no material
	for (auto &kv : parts)
	{
		const std::string &partKey = kv.first;
		PartData &pd = kv.second;

		pd.vertexToPosIndex.resize(pd.mesh.vertices.size(), -1);
		pd.vertexToSmoothGroup.resize(pd.mesh.vertices.size(), -1);
		for (const auto &entry : pd.vertexMap)
		{
			pd.vertexToPosIndex[entry.second] = entry.first.vIdx;
			pd.vertexToSmoothGroup[entry.second] = entry.first.smoothingGroup;
		}

		// Generate normals if needed
		if (normals.empty() && !pd.accumNormalsByGroup.empty()) {
			for (size_t vi = 0; vi < pd.mesh.vertices.size(); ++vi) {
				int pIdx = pd.vertexToPosIndex[vi];
				int sGroup = pd.vertexToSmoothGroup[vi];
				if (pIdx >= 0 && sGroup > 0) {
					auto it = pd.accumNormalsByGroup.find(sGroup);
					if (it == pd.accumNormalsByGroup.end())
						continue; // no accumulator for this smoothing group (should not happen)
					MATF n = MATF::normalize(it->second[pIdx]); // smoothed normal
					pd.mesh.vertices[vi].normal[0] = n.getElem(0);
					pd.mesh.vertices[vi].normal[1] = n.getElem(1);
					pd.mesh.vertices[vi].normal[2] = n.getElem(2);
				}
			}
		}

		// Generate UVs if none provided globally
		// TODO: Might need to modify this to fit the subject's requirements
		if (texcoords.empty()) {
			float yRange = (maxY - minY);
			if (yRange == 0.f) yRange = 1.f;
			for (size_t vi = 0; vi < pd.mesh.vertices.size(); ++vi) {
				int pIdx = pd.vertexToPosIndex[vi];
				if (pIdx >= 0) {
					const MATF &p = positions[pIdx];
					float x = p.getElem(0);
					float y = p.getElem(1);
					float z = p.getElem(2);
					constexpr float INV_TWO_PI = 1.0f / (2.0f * 3.14159265358979323846f);
					float u = 0.5f + std::atan2(z, x) * INV_TWO_PI;
					float v = (y - minY) / yRange;
					pd.mesh.vertices[vi].texCoords[0] = u;
					pd.mesh.vertices[vi].texCoords[1] = v;
				}
			}
		}

		// Check if normals are inverted
		if (!normals.empty() && pd.mesh.indices.size() >= 3 && pd.mesh.vertices.size() > 0) {
			MATF centroid = Transform::vec3(0.f, 0.f, 0.f);
			for (const auto &v : pd.mesh.vertices) {
				centroid = centroid + Transform::vec3(v.position[0], v.position[1], v.position[2]);
			}
			centroid = centroid * (1.0f / static_cast<float>(pd.mesh.vertices.size()));
			
			int outwardCount = 0;
			int inwardCount = 0;
			
			// Check if normals point away from centroid
			size_t sampleStep = std::max(size_t(1), pd.mesh.indices.size() / 300);
			for (size_t idx = 0; idx + 2 < pd.mesh.indices.size(); idx += sampleStep * 3) {
				GLuint i0 = pd.mesh.indices[idx];
				
				MATF p0 = Transform::vec3(
					pd.mesh.vertices[i0].position[0],
					pd.mesh.vertices[i0].position[1],
					pd.mesh.vertices[i0].position[2]);
				
				MATF toSurface = p0 - centroid;
				
				MATF vn = Transform::vec3(
					pd.mesh.vertices[i0].normal[0],
					pd.mesh.vertices[i0].normal[1],
					pd.mesh.vertices[i0].normal[2]);
				
				// If normal points away from centroid, dot > 0 (outward)
				// If normal points toward centroid, dot < 0 (inward/inverted)
				float dot = MATF::dot(MATF::normalize(toSurface), MATF::normalize(vn));
				if (dot > 0.1f) outwardCount++;
				else if (dot < -0.1f) inwardCount++;
			}
			
			// If majority point inward, they're inverted
			if (inwardCount > outwardCount) {
				/* DEBUG
				std::cout << "  Detected inverted normals in part " << partKey 
					<< " (inward: " << inwardCount << " vs outward: " << outwardCount 
					<< "), flipping..." << std::endl;
				*/
				for (auto &vert : pd.mesh.vertices) {
					vert.normal[0] = -vert.normal[0];
					vert.normal[1] = -vert.normal[1];
					vert.normal[2] = -vert.normal[2];
				}
			}
		}

		// Extract group and material from key
		//std::cout << "PART KEY: " << partKey << std::endl; // DEBUG
		size_t sep = partKey.find('|');
		std::string g = (sep == std::string::npos) ? partKey : partKey.substr(0, sep);
		std::string m = (sep == std::string::npos) ? std::string() : partKey.substr(sep+1);
		ObjModelData::Part part;
		part.name = g;
		if (m.empty() || model.materials.find(m) == model.materials.end())
		{
			if (actualDefMatName.empty())
			{
				// Add default material if not already added
				int defMatId = 0;
				const std::string defMatName = "default_mat_";
				actualDefMatName = defMatName + std::to_string(defMatId);
				while (model.materials.find(actualDefMatName) != model.materials.end()) {
					defMatId++;
					actualDefMatName = defMatName + std::to_string(defMatId);
				}
				model.materials[actualDefMatName] = Material();
				Material& defMat = model.materials[actualDefMatName];
				defMat.presetPearl();
				defMat.setIlluminationModel(ILLUM_MODEL_HIGHLIGHT_ON);
				//std::cout << "  Warning: part '" << g << "' has no material, assigning default material '" << actualDefMatName << "'" << std::endl; // DEBUG
			}
			m = actualDefMatName;
		}
		part.materialName = m;
		part.mesh = std::move(pd.mesh);
		model.parts.push_back(std::move(part));
	}

	return model;
}
