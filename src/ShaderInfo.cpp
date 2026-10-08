/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 16:39:50 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/03 14:29:49 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderInfo.hpp"

/* Private Methods */

void	ShaderInfo::updateKey() const
{
	ShaderKey newKey = "";

	if (isCustom)
		newKey += "c." + std::to_string(id) + "-";
	else
		newKey += "i." + std::to_string(static_cast<size_t>(model)) + "-";

	newKey += std::to_string(dirLightCount) + ".";
	newKey += std::to_string(pointLightCount) + ".";
	newKey += std::to_string(spotlightCount) + "-";

	newKey += (hasAmbientMap ? "1." : "0.");
	newKey += (hasDiffuseMap ? "1." : "0.");
	newKey += (hasSpecularMap ? "1." : "0.");
	newKey += (hasEmissionMap ? "1" : "0");

	_key = newKey;
	_needKeyUpdate = false;
}

/* Public Methods */

ShaderInfo::ShaderInfo()
{
	updateKey();
}

ShaderInfo::ShaderInfo(const ShaderInfo &other)
{
	*this = other;
}

ShaderInfo::ShaderInfo(bool isCustomShader)
{
	this->isCustom = isCustomShader;

	updateKey();
}

// Setters

void	ShaderInfo::makeCustom(size_t newId)
{
	isCustom = true;
	id = newId;
	_needKeyUpdate = true;
}

void	ShaderInfo::makeInternal(IlluminationModel newModel)
{
	isCustom = false;
	model = newModel;
	_needKeyUpdate = true;
}

void	ShaderInfo::setDirLightCount(unsigned int count)
{
	dirLightCount = count;
	_needKeyUpdate = true;
}

void	ShaderInfo::setPointLightCount(unsigned int count)
{
	pointLightCount = count;
	_needKeyUpdate = true;
}

void	ShaderInfo::setSpotlightCount(unsigned int count)
{
	spotlightCount = count;
	_needKeyUpdate = true;
}

void	ShaderInfo::setLightCounts(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount)
{
	dirLightCount = dirCount;
	pointLightCount = pointCount;
	spotlightCount = spotCount;
	_needKeyUpdate = true;
}

void	ShaderInfo::setHasAmbientMap(bool hasMap)
{
	hasAmbientMap = hasMap;
	_needKeyUpdate = true;
}

void	ShaderInfo::setHasDiffuseMap(bool hasMap)
{
	hasDiffuseMap = hasMap;
	_needKeyUpdate = true;
}

void	ShaderInfo::setHasSpecularMap(bool hasMap)
{
	hasSpecularMap = hasMap;
	_needKeyUpdate = true;
}

void	ShaderInfo::setHasEmissionMap(bool hasMap)
{
	hasEmissionMap = hasMap;
	_needKeyUpdate = true;
}

void	ShaderInfo::setMaps(bool ambientMap, bool diffuseMap, bool specularMap, bool emissionMap)
{
	hasAmbientMap = ambientMap;
	hasDiffuseMap = diffuseMap;
	hasSpecularMap = specularMap;
	hasEmissionMap = emissionMap;
	_needKeyUpdate = true;
}

// Getters

bool	ShaderInfo::isCustomShader() const
{
	return (isCustom);
}

bool	ShaderInfo::isInternalShader() const
{
	return (!isCustom);
}

size_t	ShaderInfo::getId() const
{
	return (id);
}

IlluminationModel	ShaderInfo::getIlluminationModel() const
{
	return (model);
}

unsigned int	ShaderInfo::getDirLightCount() const
{
	return (dirLightCount);
}

unsigned int	ShaderInfo::getPointLightCount() const
{
	return (pointLightCount);
}

unsigned int	ShaderInfo::getSpotlightCount() const
{
	return (spotlightCount);
}

bool	ShaderInfo::getHasAmbientMap() const
{
	return (hasAmbientMap);
}

bool	ShaderInfo::getHasDiffuseMap() const
{
	return (hasDiffuseMap);
}

bool	ShaderInfo::getHasSpecularMap() const
{
	return (hasSpecularMap);
}

bool	ShaderInfo::getHasEmissionMap() const
{
	return (hasEmissionMap);
}

ShaderKey	ShaderInfo::getKey() const
{
	if (_needKeyUpdate)
		updateKey();
	return (_key);
}

std::string	ShaderInfo::getFileName(ShaderType type) const
{
	ShaderKey key = getKey();

	switch (type)
	{
		case SHADER_TYPE_VERTEX:
			return key + ".vert.glsl";
		case SHADER_TYPE_FRAGMENT:
			return key + ".frag.glsl";
		default:
			throw std::invalid_argument("SHADERINFO: Invalid shader type for filename generation");
	}
}

std::string	ShaderInfo::getTemplateFileName(ShaderType type) const
{
	ShaderKey key = getKey();

	// Only keep info part for template filenames
	size_t dashPos = key.find('-');
	if (dashPos != std::string::npos)
		key = key.substr(0, dashPos);

	switch (type)
	{
		case SHADER_TYPE_VERTEX:
			return key + ".vert.glsl";
		case SHADER_TYPE_FRAGMENT:
			return key + ".frag.glsl";
		default:
			throw std::invalid_argument("SHADERINFO: Invalid shader type for template filename generation");
	}
}

// Operators

ShaderInfo&	ShaderInfo::operator=(const ShaderInfo &other)
{
	if (this != &other)
	{
		if (other.isCustomShader())
			makeCustom(other.getId());
		else
			makeInternal(other.getIlluminationModel());

		setDirLightCount(other.getDirLightCount());
		setPointLightCount(other.getPointLightCount());
		setSpotlightCount(other.getSpotlightCount());

		setHasAmbientMap(other.getHasAmbientMap());
		setHasDiffuseMap(other.getHasDiffuseMap());
		setHasSpecularMap(other.getHasSpecularMap());
		setHasEmissionMap(other.getHasEmissionMap());

		updateKey();
	}
	return (*this);
}

std::strong_ordering	ShaderInfo::operator<=>(const ShaderInfo &other) const
{
	return (this->getKey() <=> other.getKey());
}

bool	ShaderInfo::operator==(const ShaderInfo &other) const
{
	return (this->getKey() == other.getKey());
}

// This compares this ShaderInfo with another one
// BUT only takes into account the fields that matter for material matching
// so illumination model and maps (lights are ignored)
bool	ShaderInfo::doesMaterialMatch(const ShaderInfo &other) const
{
	if (isCustom != other.isCustomShader())
		return (false);
	if (isCustom && id != other.getId())
		return (false);
	if (!isCustom && model != other.getIlluminationModel())
		return (false);

	return (
		hasAmbientMap == other.getHasAmbientMap()
		&& hasDiffuseMap == other.getHasDiffuseMap()
		&& hasSpecularMap == other.getHasSpecularMap()
		&& hasEmissionMap == other.getHasEmissionMap()
	);
}

// This compares this ShaderInfo with a Material
// It only takes into account the fields that matter for material matching
// so illumination model and maps (lights are ignored)
bool	ShaderInfo::doesMaterialMatch(const Material &mat) const
{
	ShaderInfo	matShaderInfo = mat.getShaderInfo();
	return (doesMaterialMatch(matShaderInfo));
}

// Debugging

void	ShaderInfo::print() const
{
	std::cout << "ShaderInfo:" << std::endl;
	std::cout << "  type: " << (isCustom ? "Custom" : "Internal") << std::endl;
	if (isCustom)
		std::cout << "  id: " << id << std::endl;
	else
		std::cout << "  model: " << static_cast<size_t>(model) << std::endl;
	std::cout << "  dirLightCount: " << dirLightCount << std::endl;
	std::cout << "  pointLightCount: " << pointLightCount << std::endl;
	std::cout << "  spotlightCount: " << spotlightCount << std::endl;
	std::cout << "  hasAmbientMap: " << (hasAmbientMap ? "yes" : "no") << std::endl;
	std::cout << "  hasDiffuseMap: " << (hasDiffuseMap ? "yes" : "no") << std::endl;
	std::cout << "  hasSpecularMap: " << (hasSpecularMap ? "yes" : "no") << std::endl;
	std::cout << "  hasEmissionMap: " << (hasEmissionMap ? "yes" : "no") << std::endl;
}

/* External functions */

static std::vector<std::string>	split(const std::string &str, char delimiter)
{
	std::vector<std::string>	tokens;
	size_t						prevPos = 0;
	size_t						pos = str.find(delimiter);

	while (pos != std::string::npos)
	{
		tokens.push_back(str.substr(prevPos, pos - prevPos));
		prevPos = pos + 1;
		pos = str.find(delimiter, prevPos);
	}
	tokens.push_back(str.substr(prevPos));

	return (tokens);
}

static void	lengthCheck(const std::string &str, size_t expectedLength, const std::string &errorMessage)
{
	if (str.length() != expectedLength)
		throw std::invalid_argument(errorMessage);
}

static void numCheck(const std::string &str, const std::string &errorMessage)
{
	for (char c : str)
		if (!std::isdigit(c))
			throw std::invalid_argument(errorMessage);
}

static void boolCheck(const std::string &str, const std::string &errorMessage)
{
	if (str != "0" && str != "1")
		throw std::invalid_argument(errorMessage);
}

ShaderInfo	ShaderInfo::createShaderInfoFromFilename(const std::string &filename)
{
	ShaderInfo	info(false); // Temporary initialization

	// Cut into 3 parts : info, lights and maps
	std::vector<std::string>	parts = split(filename, '-');
	if (parts.size() != 3)
		throw std::invalid_argument("SHADERINFO: Invalid shader filename format");

	const std::string	infoPart = parts[0];
	const std::string	lightsPart = parts[1];
	const size_t		extensionStart = parts[2].rfind(".");
	const std::string	extension = parts[2].substr(extensionStart);
	const std::string	mapsPart = parts[2].substr(0, parts[2].size() - extension.size());

	//std::cout << "INFO PART: " << infoPart << std::endl; // DEBUG
	//std::cout << "LIGHTS PART: " << lightsPart << std::endl; // DEBUG
	//std::cout << "MAPS PART: " << mapsPart << std::endl; // DEBUG
	//std::cout << "EXTENSION: " << extension << std::endl; // DEBUG

	// Parse info part into string array
	std::vector<std::string>	infoTokens = split(infoPart, '.');
	if (infoTokens.size() != 2)
		throw std::invalid_argument("SHADERINFO: Invalid info part in shader filename");

	lengthCheck(infoTokens[0], 1, "SHADERINFO: Invalid info token length for shader type");
	numCheck(infoTokens[1], "SHADERINFO: Non-numeric value in info token for shader ID/model");

	// DEBUG
	//for (const auto &token : infoTokens)
	//	std::cout << "INFO TOKEN: " << token << std::endl;

	// Parse lights part into string array
	std::vector<std::string>	lightTokens = split(lightsPart, '.');
	if (lightTokens.size() != 3)
		throw std::invalid_argument("SHADERINFO: Invalid lights part in shader filename");

	numCheck(lightTokens[0], "SHADERINFO: Non-numeric value in lights token for directional light count");
	numCheck(lightTokens[1], "SHADERINFO: Non-numeric value in lights token for point light count");
	numCheck(lightTokens[2], "SHADERINFO: Non-numeric value in lights token for spotlight count");

	// DEBUG
	//for (const auto &token : lightTokens)
	//	std::cout << "LIGHT TOKEN: " << token << std::endl;

	// Parse maps part into string array
	std::vector<std::string>	mapTokens = split(mapsPart, '.');
	if (mapTokens.size() != 4)
		throw std::invalid_argument("SHADERINFO: Invalid maps part in shader filename");

	boolCheck(mapTokens[0], "SHADERINFO: Invalid map flag for ambient map");
	boolCheck(mapTokens[1], "SHADERINFO: Invalid map flag for diffuse map");
	boolCheck(mapTokens[2], "SHADERINFO: Invalid map flag for specular map");
	boolCheck(mapTokens[3], "SHADERINFO: Invalid map flag for emission map");

	// DEBUG
	//for (const auto &token : mapTokens)
	//	std::cout << "MAP TOKEN: " << token << std::endl;

	if (infoTokens[0] == "c")
	{
		size_t id = static_cast<size_t>(std::stoul(infoTokens[1]));
		info.makeCustom(id);
	}
	else if (infoTokens[0] == "i")
	{
		IlluminationModel model = static_cast<IlluminationModel>(std::stoul(infoTokens[1]));
		info.makeInternal(model);
	}
	else
	{
		throw std::invalid_argument("SHADERINFO: Invalid shader type in info part");
	}

	try {
		info.setDirLightCount(static_cast<unsigned int>(std::stoul(lightTokens[0])));
		info.setPointLightCount(static_cast<unsigned int>(std::stoul(lightTokens[1])));
		info.setSpotlightCount(static_cast<unsigned int>(std::stoul(lightTokens[2])));
	} catch (const std::exception &e) {
		throw std::invalid_argument("SHADERINFO: Invalid light counts in lights part");
	}

	try {
		info.setHasAmbientMap(std::stoul(mapTokens[0]) != 0);
		info.setHasDiffuseMap(std::stoul(mapTokens[1]) != 0);
		info.setHasSpecularMap(std::stoul(mapTokens[2]) != 0);
		info.setHasEmissionMap(std::stoul(mapTokens[3]) != 0);
	} catch (const std::exception &e) {
		throw std::invalid_argument("SHADERINFO: Invalid map flags in maps part");
	}

	return info;
}

/*
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <shader_filename>" << std::endl;
		return 1;
	}

	try {
		ShaderInfo test = ShaderInfo::createShaderInfoFromFilename(argv[1]);
		test.print();
		std::string filenameVert = test.getFileName(SHADER_TYPE_VERTEX);
		std::string filenameFrag = test.getFileName(SHADER_TYPE_FRAGMENT);
		std::cout << "Reconstructed vertex filename: " << filenameVert << std::endl;
		std::cout << "Reconstructed fragment filename: " << filenameFrag << std::endl;
		std::string templateFilenameVert = test.getTemplateFileName(SHADER_TYPE_VERTEX);
		std::string templateFilenameFrag = test.getTemplateFileName(SHADER_TYPE_FRAGMENT);
		std::cout << "Template vertex filename: " << templateFilenameVert << std::endl;
		std::cout << "Template fragment filename: " << templateFilenameFrag << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
*/