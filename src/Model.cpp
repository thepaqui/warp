/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Model.cpp										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: thepaqui <thepaqui@student.42nice.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/01/10 13:55:13 by thepaqui		  #+#	#+#			 */
/*   Updated: 2025/11/07 14:19:13 by thepaqui		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Model.hpp"

const MATF& Model::getSize() const
{
	return _size;
}

void Model::presetPlane(Material&& material, const float textureRepeat)
{
	std::vector<Vertex> vertices = {
		// Top
		{{-0.5f, 0.0f, -0.5f}, {0.f,  1.f, 0.f}, {0.f, 0.f}},
		{{ 0.5f, 0.0f, -0.5f}, {0.f,  1.f, 0.f}, {textureRepeat, 0.f}},
		{{ 0.5f, 0.0f,  0.5f}, {0.f,  1.f, 0.f}, {textureRepeat, textureRepeat}},
		{{-0.5f, 0.0f,  0.5f}, {0.f,  1.f, 0.f}, {0.f, textureRepeat}},

		// Bottom
		{{-0.5f, 0.0f, -0.5f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{ 0.5f, 0.0f, -0.5f}, {0.f, -1.f, 0.f}, {textureRepeat, 0.f}},
		{{ 0.5f, 0.0f,  0.5f}, {0.f, -1.f, 0.f}, {textureRepeat, textureRepeat}},
		{{-0.5f, 0.0f,  0.5f}, {0.f, -1.f, 0.f}, {0.f, textureRepeat}},
	};

	// 4 tris to avoid face culling issues (might disappear in some cases)
	std::vector<GLuint> indices = {
		// Top
		0, 2, 1,
		0, 3, 2,

		// Bottom
		4, 5, 6,
		4, 6, 7
	};

	_materials.clear();
	_materials["presetPlaneMat"] = std::move(material);

	_parts.clear();
	_parts.emplace_back(Mesh(vertices, indices), "presetPlane", std::string("presetPlaneMat"));

	// Set size for preset plane (1.0 x 0.0 x 1.0)
	_size = TVEC3(1.0f, 0.0f, 1.0f);
}

void Model::presetCube(Material&& material)
{
	std::vector<Vertex> vertices = {
		// Front face
		{{-0.5f, -0.5f,  0.5f}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
		{{-0.5f,  0.5f,  0.5f}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
		// Back face
		{{-0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {1.f, 0.f}},
		{{-0.5f,  0.5f, -0.5f}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 1.f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
		// Left face
		{{-0.5f, -0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.f, 0.f, 0.f}, {1.f, 0.f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {0.f, 1.f}},
		// Right face
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 0.f, 0.f}, {1.f, 1.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 0.f, 0.f}, {0.f, 1.f}},
		// Top face
		{{-0.5f,  0.5f,  0.5f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
		{{-0.5f,  0.5f, -0.5f}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
		// Bottom face
		{{-0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
		{{-0.5f, -0.5f,  0.5f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
	};

	std::vector<GLuint> indices = {
		0, 1, 2, 2, 3, 0,	// Front face
		4, 5, 6, 6, 7, 4,	// Back face
		8, 9,10,10,11, 8,	// Left face
		12,13,14,14,15,12,	// Right face
		16,17,18,18,19,16,	// Top face
		20,21,22,22,23,20	// Bottom face
	};

	_materials.clear();
	_materials["presetCubeMat"] = std::move(material);

	_parts.clear();
	_parts.emplace_back(Mesh(vertices, indices), "presetCube", std::string("presetCubeMat"));

	// Set size for preset cube (1.0 x 1.0 x 1.0)
	_size = TVEC3(1.0f, 1.0f, 1.0f);
}

void Model::presetCubeWrap(Material&& material)
{
	const float	third = 1.0f / 3.0f;
	const float	third2 = 2.0f / 3.0f;

	std::vector<Vertex> vertices = {
		// Front face
		{{-0.5f, -0.5f,  0.5f}, {0.f, 0.f, 1.f}, {0.f, 0.5f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.f, 0.f, 1.f}, {third, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.f, 0.f, 1.f}, {third, 1.f}},
		{{-0.5f,  0.5f,  0.5f}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
		// Back face
		{{-0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {third, 0.f}},
		{{-0.5f,  0.5f, -0.5f}, {0.f, 0.f, -1.f}, {third, 0.5f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 0.5f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
		// Left face
		{{-0.5f, -0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {third, 0.f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.f, 0.f, 0.f}, {third2, 0.f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.f, 0.f, 0.f}, {third2, 0.5f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {third, 0.5f}},
		// Right face
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 0.f, 0.f}, {third, 0.5f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f, 0.f}, {third2, 0.5f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 0.f, 0.f}, {third2, 1.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 0.f, 0.f}, {third, 1.f}},
		// Top face
		{{-0.5f,  0.5f,  0.5f}, {0.f, 1.f, 0.f}, {third2, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.f, 1.f, 0.f}, {1.f, 0.5f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
		{{-0.5f,  0.5f, -0.5f}, {0.f, 1.f, 0.f}, {third2, 1.f}},
		// Bottom face
		{{-0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {1.f, 0.5f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {third2, 0.5f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.f, -1.f, 0.f}, {third2, 0.f}},
		{{-0.5f, -0.5f,  0.5f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
	};

	std::vector<GLuint> indices = {
		0, 1, 2, 2, 3, 0,	// Front face
		4, 5, 6, 6, 7, 4,	// Back face
		8, 9,10,10,11, 8,	// Left face
		12,13,14,14,15,12,	// Right face
		16,17,18,18,19,16,	// Top face
		20,21,22,22,23,20	// Bottom face
	};

	_materials.clear();
	_materials["presetCubeWrapMat"] = std::move(material);

	_parts.clear();
	_parts.emplace_back(Mesh(vertices, indices), "presetCubeWrap", std::string("presetCubeWrapMat"));

	// Set size for preset cube wrap (1.0 x 1.0 x 1.0)
	_size = TVEC3(1.0f, 1.0f, 1.0f);
}

// TODO : delete (?)
void Model::draw(ShaderProgram &shader)
{
	for (auto &part : _parts) {
		const Material* mat = nullptr;
		if (!part.materialName.empty()) {
			auto it = _materials.find(part.materialName);
			if (it != _materials.end())
				mat = &it->second;
		}
		//std::cout << "Drawing part " << part.name << " with material " << (mat ? part.materialName : "(none)") << std::endl; // debug
		part.mesh.draw(shader, mat);
	}
}

void Model::drawOnly(ShaderProgram &shader, const ShaderInfo& shaderInfo)
{
	for (auto &part : _parts) {
		const Material* mat = nullptr;
		if (!part.materialName.empty()) {
			auto it = _materials.find(part.materialName);
			if (it != _materials.end())
				mat = &it->second;
		}
		if (!mat || !shaderInfo.doesMaterialMatch(*mat))
			continue;

		//std::cout << "Drawing part " << part.name << " with material " << (mat ? part.materialName : "(none)") << std::endl; // debug
		part.mesh.draw(shader, shaderInfo, mat);
	}
}

void Model::measureModel(ObjModelData &data)
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto &part : data.parts) {
		for (const auto &v : part.mesh.vertices) {
			float x = v.position[0];
			float y = v.position[1];
			float z = v.position[2];
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
	}

	float sizeX = fabs(maxX - minX);
	float sizeY = fabs(maxY - minY);
	float sizeZ = fabs(maxZ - minZ);

	_size.setElem(0, sizeX);
	_size.setElem(1, sizeY);
	_size.setElem(2, sizeZ);
}

void Model::centerAndMeasureModel(ObjModelData &data)
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto &part : data.parts) {
		for (const auto &v : part.mesh.vertices) {
			float x = v.position[0];
			float y = v.position[1];
			float z = v.position[2];
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
	}

	float sizeX = fabs(maxX - minX);
	float sizeY = fabs(maxY - minY);
	float sizeZ = fabs(maxZ - minZ);

	_size.setElem(0, sizeX);
	_size.setElem(1, sizeY);
	_size.setElem(2, sizeZ);

	float cx = (minX + maxX) * 0.5f;
	float cy = (minY + maxY) * 0.5f;
	float cz = (minZ + maxZ) * 0.5f;

	for (auto &part : data.parts) {
		for (auto &v : part.mesh.vertices) {
			v.position[0] -= cx;
			v.position[1] -= cy;
			v.position[2] -= cz;
		}
	}
	//std::cout << "Model centered: bbox center (" << cx << ", " << cy << ", " << cz << ") subtracted" << std::endl; // debug
}

void Model::loadModel(const std::string &path, bool centerOnLoad)
{
	try {
		ObjModelData data = ObjLoader::loadModel(path);

		// Measure model size and optionally center the model
		// on load by moving vertex positions
		if (centerOnLoad)
			centerAndMeasureModel(data);
		else
			measureModel(data);

//		// DEBUG
//		std::cout << "Loaded OBJ: " << data.parts.size() << " part(s)" << std::endl;
//		for (const auto &part : data.parts)
//		{
//			std::cout << " Part: " << part.name
//				<< ", material: " << (part.materialName.empty() ? "(none)" : part.materialName)
//				<< ", vertices: " << part.mesh.vertices.size()
//				<< ", indices: " << part.mesh.indices.size()
//			<< std::endl;
//		}

		_materials = std::move(data.materials);

		_parts.clear();
		for (auto &p : data.parts) {
			Mesh mesh(p.mesh.vertices, p.mesh.indices);
			_parts.emplace_back(std::move(mesh), p.name, p.materialName);
		}
	} catch (const std::exception &e) {
		throw;
	}
}

std::vector<ShaderInfo> Model::getUsedShadersInfo() const
{
	std::vector<ShaderInfo>	result;
	std::unordered_set<ShaderInfo>	seenShaders(false);

	for (const auto &matPair : _materials)
	{
		const Material	&mat = matPair.second;

		ShaderInfo	shaderInfo = mat.getShaderInfo();
		if (seenShaders.insert(shaderInfo).second)
			result.push_back(shaderInfo);
	}

	return result;
}

void	Model::addMaterial(const std::string &name, Material&& material)
{
	if (name.empty())
		throw std::invalid_argument("[MODEL] Material name cannot be empty");
	if (_materials.find(name) != _materials.end())
		throw std::invalid_argument("[MODEL] Material name '" + name + "' already exists in the model");
	_materials[name] = std::move(material);
}

void	Model::forceMaterial(const std::string &materialName)
{
	if (materialName.empty())
		throw std::invalid_argument("[MODEL] Material name cannot be empty");
	auto search = _materials.find(materialName);
	if (search == _materials.end())
		throw std::invalid_argument("[MODEL] Material name '" + materialName + "' not found in the model");

	// Remove all other materials
	std::erase_if(_materials, [&materialName](const auto &pair) {
		return pair.first != materialName;
	});

	for (auto &part : _parts)
		part.materialName = materialName;
}

void	Model::forceIlluminationModel(const IlluminationModel newModel)
{
	for (auto &matPair : _materials)
		matPair.second.setIlluminationModel(newModel);
}