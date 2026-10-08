/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:44:25 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 17:55:13 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderManager.hpp"

ShaderManager::ShaderManager(const std::string &templateDirPath, const std::string &shaderDirPath)
{
	_templateDirPath = templateDirPath;
	_shaderDirPath = shaderDirPath;
}

ShaderManager::~ShaderManager()
{
	// Delete shaders
	for (auto &pair : _shaderPrograms)
	{
		delete pair.second;
	}
	_shaderPrograms.clear();
}

ShaderProgram&	ShaderManager::getShaderProgram(const ShaderInfo &info) const
{
	auto it = _shaderPrograms.find(info);
	if (it != _shaderPrograms.end())
		return *(it->second);

	throw ShaderManager::ShaderProgramNotFoundException();
}

bool	ShaderManager::hasShaderProgram(const ShaderInfo &info) const
{
	auto it = _shaderPrograms.find(info);
	return (it != _shaderPrograms.end());
}

static void writeInfoToShaderFile(std::ofstream &shaderFile, const ShaderInfo &info)
{
	shaderFile << "// Auto-generated shader file\n";
	shaderFile << "// Shader Info Key: " << info.getKey() << "\n\n";

	shaderFile << "#version 460 core\n\n";

	// Point lights
	shaderFile << "#define POINT_LIGHT_COUNT " << info.getPointLightCount() << "\n";
	// Directional lights
	shaderFile << "#define DIR_LIGHT_COUNT " << info.getDirLightCount() << "\n";
	// Spotlights
	shaderFile << "#define SPOT_LIGHT_COUNT " << info.getSpotlightCount() << "\n";

	// Texture maps
	shaderFile << "#define HAS_AMBIENT_MAP " << (info.getHasAmbientMap() ? "1" : "0") << "\n";
	shaderFile << "#define HAS_DIFFUSE_MAP " << (info.getHasDiffuseMap() ? "1" : "0") << "\n";
	shaderFile << "#define HAS_SPECULAR_MAP " << (info.getHasSpecularMap() ? "1" : "0") << "\n";
	shaderFile << "#define HAS_EMISSION_MAP " << (info.getHasEmissionMap() ? "1" : "0") << "\n\n";

	if (!shaderFile)
		throw ShaderManager::ShaderCreationFailedException();
}

void	ShaderManager::addShaderProgram(const ShaderInfo &info)
{
	if (hasShaderProgram(info))
		return;

	const std::string	vShaderTemplatePath = _templateDirPath + "/" + info.getTemplateFileName(SHADER_TYPE_VERTEX);
	const std::string	fShaderTemplatePath = _templateDirPath + "/" + info.getTemplateFileName(SHADER_TYPE_FRAGMENT);

	// open template files to create shader sources
	std::ifstream vShaderTemplateFile(vShaderTemplatePath, std::ios::binary);
	std::ifstream fShaderTemplateFile(fShaderTemplatePath, std::ios::binary);
	if (!vShaderTemplateFile)
		throw ShaderManager::ShaderTemplateNotFoundException(vShaderTemplatePath);
	if (!fShaderTemplateFile)
		throw ShaderManager::ShaderTemplateNotFoundException(fShaderTemplatePath);

	// create shader source files from templates in shader directory
	const std::string	vPath = _shaderDirPath + "/" + info.getFileName(SHADER_TYPE_VERTEX);
	const std::string	fPath = _shaderDirPath + "/" + info.getFileName(SHADER_TYPE_FRAGMENT);
	
	const bool vShaderExists = (std::filesystem::exists(vPath));
	const bool fShaderExists = (std::filesystem::exists(fPath));

	const std::string	vTmpPath = vPath + ".tmp";
	const std::string	fTmpPath = fPath + ".tmp";
	
	if (!vShaderExists || !fShaderExists)
	{
		std::ofstream vTmpFile(vTmpPath, std::ios::binary);
		std::ofstream fTmpFile(fTmpPath,  std::ios::binary);
		if (!vTmpFile || !fTmpFile)
			throw ShaderManager::ShaderCreationFailedException();
		
		writeInfoToShaderFile(vTmpFile, info);
		vTmpFile << vShaderTemplateFile.rdbuf();

		writeInfoToShaderFile(fTmpFile, info);
		fTmpFile << fShaderTemplateFile.rdbuf();

		if (!vTmpFile || !fTmpFile)
			throw ShaderManager::ShaderCreationFailedException();

		vTmpFile.close();
		fTmpFile.close();

		if (vShaderExists && std::remove(vPath.c_str()) != 0)
			throw ShaderManager::ShaderCreationFailedException();
		if (std::rename(vTmpPath.c_str(), vPath.c_str()) != 0)
			throw ShaderManager::ShaderCreationFailedException();

		if (fShaderExists && std::remove(fPath.c_str()) != 0)
			throw ShaderManager::ShaderCreationFailedException();
		if (std::rename(fTmpPath.c_str(), fPath.c_str()) != 0)
			throw ShaderManager::ShaderCreationFailedException();
	}

	// create ShaderProgram
	ShaderProgram* sp = nullptr;
	try {
		sp = new ShaderProgram(vPath, fPath);
		_shaderPrograms.emplace(info, sp);
	} catch (const std::exception &e) {
		if (sp)
			delete sp;
		// TODO: Log compiler errors from e.what()? Maybe?
		throw ShaderManager::ShaderProgramCreationFailedException();
	}
}

size_t	ShaderManager::addCustomShaderTemplate(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, IlluminationModelTraits illumTraits)
{
	const size_t newId = _customShadersCount;

	const std::string	vPath = _templateDirPath + "/c." + std::to_string(newId) + ".vert.glsl";
	const std::string	fPath = _templateDirPath + "/c." + std::to_string(newId) + ".frag.glsl";

	const std::string	vTmpPath = vPath + ".tmp";
	const std::string	fTmpPath = fPath + ".tmp";

	// Open source files (binary mode to avoid issues with different OSes)
	std::ifstream srcV(vertexShaderPath, std::ios::binary);
	std::ifstream srcF(fragmentShaderPath, std::ios::binary);
	if (!srcV)
		throw ShaderManager::ShaderTemplateNotFoundException(vertexShaderPath);
	if (!srcF)
		throw ShaderManager::ShaderTemplateNotFoundException(fragmentShaderPath);

	// Create temporary template files
	std::ofstream dstV(vTmpPath, std::ios::binary);
	std::ofstream dstF(fTmpPath, std::ios::binary);
	if (!dstV || !dstF)
		throw ShaderManager::ShaderTemplateCreationFailedException();

	// Copy contents
	dstV << srcV.rdbuf();
	dstF << srcF.rdbuf();

	// Verify files were created and copied correctly
	if (!dstV || !dstF)
		throw ShaderManager::ShaderTemplateCreationFailedException();

	dstV.close();
	dstF.close();

	std::rename(vTmpPath.c_str(), vPath.c_str());
	std::rename(fTmpPath.c_str(), fPath.c_str());

	_customShaderIlluminationTraits[newId] = illumTraits;

	_customShadersCount++;

	return newId;
}

bool	ShaderManager::doesCustomShaderUseNormalMatrix(const size_t customShaderId) const
{
	auto it = _customShaderIlluminationTraits.find(customShaderId);
	if (it == _customShaderIlluminationTraits.end())
		throw ShaderManager::ShaderProgramNotFoundException();
	return it->second.usesNormalMatrix;
}

bool	ShaderManager::doesCustomShaderUseAmbient(const size_t customShaderId) const
{
	auto it = _customShaderIlluminationTraits.find(customShaderId);
	if (it == _customShaderIlluminationTraits.end())
		throw ShaderManager::ShaderProgramNotFoundException();
	return it->second.usesAmbient;
}

bool	ShaderManager::doesCustomShaderUseDiffuse(const size_t customShaderId) const
{
	auto it = _customShaderIlluminationTraits.find(customShaderId);
	if (it == _customShaderIlluminationTraits.end())
		throw ShaderManager::ShaderProgramNotFoundException();
	return it->second.usesDiffuse;
}

bool	ShaderManager::doesCustomShaderUseSpecular(const size_t customShaderId) const
{
	auto it = _customShaderIlluminationTraits.find(customShaderId);
	if (it == _customShaderIlluminationTraits.end())
		throw ShaderManager::ShaderProgramNotFoundException();
	return it->second.usesSpecular;
}

bool	ShaderManager::doesCustomShaderUseEmission(const size_t customShaderId) const
{
	auto it = _customShaderIlluminationTraits.find(customShaderId);
	if (it == _customShaderIlluminationTraits.end())
		throw ShaderManager::ShaderProgramNotFoundException();
	return it->second.usesEmission;
}

bool	ShaderManager::doesCustomShaderUseShininess(const size_t customShaderId) const
{
	return doesCustomShaderUseSpecular(customShaderId);
}

bool	ShaderManager::doesCustomShaderUseCamPos(const size_t customShaderId) const
{
	return doesCustomShaderUseSpecular(customShaderId);
}