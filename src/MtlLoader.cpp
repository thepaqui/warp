/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MtlLoader.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:22:20 by Copilot           #+#    #+#             */
/*   Updated: 2026/02/10 14:07:21 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MtlLoader.hpp"

static std::string getBaseDir(const std::string &path)
{
	auto sep = path.find_last_of("/");
	return (sep == std::string::npos) ? std::string(".") : path.substr(0, sep);
}

static void parse_newmtl_tag
(
	std::istringstream &ls,
	Material* &current,
	std::string &currentName,
	std::map<std::string, Material> &materials
)
{
	ls >> currentName;
	if (currentName.empty())
		return; // Skip malformed newmtl lines
	materials.emplace(currentName, Material());
	current = &materials.at(currentName);
}

static void parse_int_tag
(
	std::istringstream &ls,
	Material &material,
	const std::string &tag
)
{
	int i;
	if (ls >> i) {
		if (tag == "illum")
			material.setIlluminationModel(i);
	}
}

static void parse_float_tag
(
	std::istringstream &ls,
	Material &material,
	const std::string &tag
)
{
	float f;
	if (ls >> f) {
		if (tag == "Ns")
			material.setShininess(f);
		else if (tag == "d")
			material.setOpacity(f);
		else if (tag == "Tr")
			material.setOpacity(1.0f - f);
		else if (tag == "Ni")
			material.setRefractiveIndex(f);
	}
}

static void parse_float_vec3_tag
(
	std::istringstream &ls,
	Material &material,
	const std::string &tag
)
{
	float r, g, b;
	if (ls >> r >> g >> b) {
		if (tag == "Ka")
			material.setAmbientColor(TVEC3(r, g, b));
		else if (tag == "Kd")
			material.setDiffuseColor(TVEC3(r, g, b));
		else if (tag == "Ks")
			material.setSpecularColor(TVEC3(r, g, b));
		else if (tag == "Ke")
			material.setEmissionColor(TVEC3(r, g, b));
		else if (tag == "Tf")
			material.setTransmissionFilter(TVEC3(r, g, b));
	}
}

static void parse_map_tag
(
	std::istringstream &ls,
	Material &material,
	const std::string &tag,
	const std::string &baseDir
)
{
	std::string tex;
	if (ls >> tex) {
		if (!tex.empty() && tex.front() != '/' && !(tex.size() > 1 && tex[1] == ':'))
			tex = baseDir + "/" + tex; // Prepend basedir if relative path

		if (tag == "map_Ka")
			material.setAmbientMap(tex.c_str());
		else if (tag == "map_Kd")
			material.setDiffuseMap(tex.c_str());
		else if (tag == "map_Ks")
			material.setSpecularMap(tex.c_str());
		else if (tag == "map_Ke")
			material.setEmissionMap(tex.c_str());
	}
}

// MTL file parser that completely or partly handles the following tags:
// - newmtl
// - illum
// - Ka, Kd, Ks, Ke, Tf
// - map_Ka, map_Kd, map_Ks, map_Ke
// - Ns, d, Tr, Ni
void parseMTL(const std::string &mtlPath, std::map<std::string, Material> &out)
{
	std::ifstream in(mtlPath);
	if (!in.good())
		return; // silently ignore missing MTL

	std::string line;
	Material *current = nullptr;
	std::string currentName;
	// Directory containing the MTL, used to resolve relative texture paths
	const std::string baseDir = getBaseDir(mtlPath);

	while (std::getline(in, line)) {
		size_t start = line.find_first_not_of(" \t\r"); // Skip leading whitespace
		if (start == std::string::npos) continue; // Skip empty lines
		if (line[start] == '#') continue; // Skip comments

		std::istringstream ls(line);
		std::string tag;
		ls >> tag;
		if (tag == "newmtl") {
			// Creating a new material
			parse_newmtl_tag(ls, current, currentName, out);
		} else if (!current) {
			continue; // Skipping material property lines before having created an actual material
		} else if (
			tag == "Ka"
			or tag == "Kd"
			or tag == "Ks"
			or tag == "Ke"
			or tag == "Tf"
		) {
			// Ambient, diffuse, specular, emission color
			parse_float_vec3_tag(ls, *current, tag);
		} else if (
			tag == "map_Ka"
			or tag == "map_Kd"
			or tag == "map_Ks"
			or tag == "map_Ke"
		) {
			// Ambient, diffuse, specular, emission map
			parse_map_tag(ls, *current, tag, baseDir);
		} else if (
			tag == "d"
			or tag == "Tr"
			or tag == "Ns"
			or tag == "Ni"
		) {
			// Shininess, opacity and refractive index
			parse_float_tag(ls, *current, tag);
		} else if (tag == "illum") {
			// Illumination model
			parse_int_tag(ls, *current, tag);
		}
		// Ignore other tags for now (might implement normal maps later)
	}
}
