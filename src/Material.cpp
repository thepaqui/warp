/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 21:49:26 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 16:17:05 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Material.hpp"

Material::~Material()
{
	clearAmbientMap();
	clearDiffuseMap();
	clearSpecularMap();
	clearEmissionMap();
}

// Move constructor
Material::Material(Material&& other) noexcept
{
	_ambient = other._ambient;
	_diffuse = other._diffuse;
	_specular = other._specular;
	_emission = other._emission;
	_transmissionFilter = other._transmissionFilter;
	_shininess = other._shininess;
	_opacity = other._opacity;
	_refractiveIndex = other._refractiveIndex;
	_illuminationModel = other._illuminationModel;
	_customShaderID = other._customShaderID;

	// Steal texture pointers and flags
	_ambientMap = other._ambientMap;
	_usingAmbientMap = other._usingAmbientMap;
	other._ambientMap = nullptr;
	other._usingAmbientMap = false;

	_diffuseMap = other._diffuseMap;
	_usingDiffuseMap = other._usingDiffuseMap;
	other._diffuseMap = nullptr;
	other._usingDiffuseMap = false;

	_specularMap = other._specularMap;
	_usingSpecularMap = other._usingSpecularMap;
	other._specularMap = nullptr;
	other._usingSpecularMap = false;

	_emissionMap = other._emissionMap;
	_usingEmissionMap = other._usingEmissionMap;
	other._emissionMap = nullptr;
	other._usingEmissionMap = false;
}

// Move assignment
Material& Material::operator=(Material&& other) noexcept
{
	if (this != &other)
	{
		// Clean up existing resources
		clearAmbientMap();
		clearDiffuseMap();
		clearSpecularMap();
		clearEmissionMap();

		_ambient = other._ambient;
		_diffuse = other._diffuse;
		_specular = other._specular;
		_emission = other._emission;
		_transmissionFilter = other._transmissionFilter;
		_shininess = other._shininess;
		_opacity = other._opacity;
		_refractiveIndex = other._refractiveIndex;
		_illuminationModel = other._illuminationModel;
		_customShaderID = other._customShaderID;

		// Steal texture pointers and flags
		_ambientMap = other._ambientMap;
		_usingAmbientMap = other._usingAmbientMap;
		other._ambientMap = nullptr;
		other._usingAmbientMap = false;

		_diffuseMap = other._diffuseMap;
		_usingDiffuseMap = other._usingDiffuseMap;
		other._diffuseMap = nullptr;
		other._usingDiffuseMap = false;

		_specularMap = other._specularMap;
		_usingSpecularMap = other._usingSpecularMap;
		other._specularMap = nullptr;
		other._usingSpecularMap = false;

		_emissionMap = other._emissionMap;
		_usingEmissionMap = other._usingEmissionMap;
		other._emissionMap = nullptr;
		other._usingEmissionMap = false;
	}
	return *this;
}

Material::Material(const Material &obj)
{
	_ambient = obj.getAmbientColor();
	_diffuse = obj.getDiffuseColor();
	_specular = obj.getSpecularColor();
	_emission = obj.getEmissionColor();
	_transmissionFilter = obj.getTransmissionFilter();
	_shininess = obj.getShininess();
	_opacity = obj.getOpacity();
	_refractiveIndex = obj.getRefractiveIndex();
	_illuminationModel = obj.getIlluminationModel();
	_customShaderID = obj.getCustomShaderID();
	_ambientMap = nullptr;
	_usingAmbientMap = obj._usingAmbientMap;
	_diffuseMap = nullptr;
	_usingDiffuseMap = obj._usingDiffuseMap;
	_specularMap = nullptr;
	_usingSpecularMap = obj._usingSpecularMap;
	_emissionMap = nullptr;
	_usingEmissionMap = obj._usingEmissionMap;

	if (obj._ambientMap)
		_ambientMap = new Texture(*obj._ambientMap);
	if (obj._diffuseMap)
		_diffuseMap = new Texture(*obj._diffuseMap);
	if (obj._specularMap)
		_specularMap = new Texture(*obj._specularMap);
	if (obj._emissionMap)
		_emissionMap = new Texture(*obj._emissionMap);
}

Material	&Material::operator=(const Material &obj) noexcept
{
	if (this != &obj)
	{
		_ambient = obj.getAmbientColor();
		_diffuse = obj.getDiffuseColor();
		_specular = obj.getSpecularColor();
		_emission = obj.getEmissionColor();
		_transmissionFilter = obj.getTransmissionFilter();
		_shininess = obj.getShininess();
		_opacity = obj.getOpacity();
		_refractiveIndex = obj.getRefractiveIndex();
		_illuminationModel = obj.getIlluminationModel();
		_customShaderID = obj.getCustomShaderID();

		clearAmbientMap();
		clearDiffuseMap();
		clearSpecularMap();
		clearEmissionMap();

		_usingAmbientMap = obj._usingAmbientMap;
		_usingDiffuseMap = obj._usingDiffuseMap;
		_usingSpecularMap = obj._usingSpecularMap;
		_usingEmissionMap = obj._usingEmissionMap;

		if (obj._ambientMap)
			_ambientMap = new Texture(*obj._ambientMap);
		if (obj._diffuseMap)
			_diffuseMap = new Texture(*obj._diffuseMap);
		if (obj._specularMap)
			_specularMap = new Texture(*obj._specularMap);
		if (obj._emissionMap)
			_emissionMap = new Texture(*obj._emissionMap);
	}
	return (*this);
}

void	Material::printInfo() const
{
	std::cout << "Material info :" << std::endl;
	std::cout << "- Illumination model : " << static_cast<int>(_illuminationModel) << std::endl;
	std::cout << "- Ambient color : (" << _ambient.getElem(0) << ", "
		<< _ambient.getElem(1) << ", " << _ambient.getElem(2) << ")" << std::endl;
	std::cout << "- Diffuse color : (" << _diffuse.getElem(0) << ", "
		<< _diffuse.getElem(1) << ", " << _diffuse.getElem(2) << ")" << std::endl;
	std::cout << "- Specular color : (" << _specular.getElem(0) << ", "
		<< _specular.getElem(1) << ", " << _specular.getElem(2) << ")" << std::endl;
	std::cout << "- Emission color : (" << _emission.getElem(0) << ", "
		<< _emission.getElem(1) << ", " << _emission.getElem(2) << ")" << std::endl;
	std::cout << "- Transmission filter : (" << _transmissionFilter.getElem(0) << ", "
		<< _transmissionFilter.getElem(1) << ", " << _transmissionFilter.getElem(2) << ")" << std::endl;
	std::cout << "- Shininess : " << _shininess << std::endl;
	std::cout << "- Opacity : " << _opacity << std::endl;
	std::cout << "- Refractive index : " << _refractiveIndex << std::endl;
	std::cout << "- Using ambient map : " << (_usingAmbientMap ? "Yes" : "No") << std::endl;
	std::cout << "- Using diffuse map : " << (_usingDiffuseMap ? "Yes" : "No") << std::endl;
	std::cout << "- Using specular map : " << (_usingSpecularMap ? "Yes" : "No") << std::endl;
	std::cout << "- Using emission map : " << (_usingEmissionMap ? "Yes" : "No") << std::endl;
	std::cout << "- Ambient map : " << _ambientMap << std::endl;
	std::cout << "- Diffuse map : " << _diffuseMap << std::endl;
	std::cout << "- Specular map : " << _specularMap << std::endl;
	std::cout << "- Emission map : " << _emissionMap << std::endl;
}

// Generates ShaderInfo corresponding to this material
// Does not include light counts, as they are managed at the scene level
ShaderInfo	Material::getShaderInfo() const
{
	ShaderInfo	info(false);

	if (_customShaderID.has_value())
		info.makeCustom(_customShaderID.value());
	else
		info.makeInternal(_illuminationModel);
	info.setMaps(
		_usingAmbientMap,
		_usingDiffuseMap,
		_usingSpecularMap,
		_usingEmissionMap
	);

	return info;
}

// TODO: delete ?
void	Material::apply(ShaderProgram &shader) const
{
	// Ambient
	shader.setBool("material.hasAmbientMap", _usingAmbientMap);
	if (_usingAmbientMap)
	{
		useAmbientMap();
		shader.setInt("material.ambientMap", TEXTURE_UNIT_AMBIENT);
	}
	else
	{
		shader.setFloatVec3("material.ambientColor", _ambient);
	}

	// Diffuse
	shader.setBool("material.hasDiffuseMap", _usingDiffuseMap);
	if (_usingDiffuseMap)
	{
		useDiffuseMap();
		shader.setInt("material.diffuseMap", TEXTURE_UNIT_DIFFUSE);
	}
	else
	{
		shader.setFloatVec3("material.diffuseColor", _diffuse);
	}

	// Specular
	shader.setBool("material.hasSpecularMap", _usingSpecularMap);
	if (_usingSpecularMap)
	{
		useSpecularMap();
		shader.setInt("material.specularMap", TEXTURE_UNIT_SPECULAR);
	}
	else
	{
		shader.setFloatVec3("material.specularColor", _specular);
	}

	// Emission
	shader.setBool("material.hasEmissionMap", _usingEmissionMap);
	if (_usingEmissionMap)
	{
		useEmissionMap();
		shader.setInt("material.emissionMap", TEXTURE_UNIT_EMISSION);
	}
	else
	{
		shader.setFloatVec3("material.emissionColor", _emission);
	}

	// Shininess
	shader.setFloat("material.shininess", _shininess);
}

void	Material::apply(ShaderProgram& shader, const ShaderInfo &shaderInfo) const
{
	const IlluminationModel	model = shaderInfo.getIlluminationModel();

	// Ambient
	if (doesIlluminationModelUseAmbient(model))
	{
		if (shaderInfo.getHasAmbientMap())
		{
			useAmbientMap();
			shader.setInt("material.ambientMap", TEXTURE_UNIT_AMBIENT);
		}
		else
		{
			shader.setFloatVec3("material.ambientColor", _ambient);
		}
	}

	// Diffuse
	if (doesIlluminationModelUseDiffuse(model))
	{
		if (shaderInfo.getHasDiffuseMap())
		{
			useDiffuseMap();
			shader.setInt("material.diffuseMap", TEXTURE_UNIT_DIFFUSE);
		}
		else
		{
			shader.setFloatVec3("material.diffuseColor", _diffuse);
		}
	}

	// Specular
	if (doesIlluminationModelUseSpecular(model))
	{
		if (shaderInfo.getHasSpecularMap())
		{
			useSpecularMap();
			shader.setInt("material.specularMap", TEXTURE_UNIT_SPECULAR);
		}
		else
		{
			shader.setFloatVec3("material.specularColor", _specular);
		}
	}

	// Emission
	if (doesIlluminationModelUseEmission(model))
	{
		if (shaderInfo.getHasEmissionMap())
		{
			useEmissionMap();
			shader.setInt("material.emissionMap", TEXTURE_UNIT_EMISSION);
		}
		else
		{
			shader.setFloatVec3("material.emissionColor", _emission);
		}
	}

	// Only set shininess for models that use it
	if (doesIlluminationModelUseShininess(model))
		shader.setFloat("material.shininess", _shininess);

	if (doesIlluminationModelUseLightColor(model))
		shader.setFloatVec3("lightColor", _emission);
}

void	Material::apply(const ShaderManager& shaderManager, const ShaderInfo &shaderInfo) const
{
	ShaderProgram&	shader = shaderManager.getShaderProgram(shaderInfo);
	apply(shader, shaderInfo);
}

// Ambient map management

void	Material::setAmbientMap(
	const char* texturePath,
	const bool mipmap,
	const bool repeatS,
	const bool repeatT,
	const GLint minfilter,
	const GLint magfilter
)
{
	clearAmbientMap();
	_ambientMap = new Texture(texturePath, TEXTURE_TYPE_AMBIENT);
	_usingAmbientMap = true;

	_ambientMap->setTextureMinFilter(minfilter);
	_ambientMap->setTextureMagFilter(magfilter);
	if (repeatS)
		_ambientMap->setTextureWrapS(GL_REPEAT);
	if (repeatT)
		_ambientMap->setTextureWrapT(GL_REPEAT);
	if (mipmap)
		_ambientMap->genMipmap();
}

void	Material::useAmbientMap() const
{
	if (_usingAmbientMap && _ambientMap)
	{
		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_AMBIENT);
		_ambientMap->use();
	}
}

void	Material::clearAmbientMap()
{
	if (_ambientMap)
	{
		delete _ambientMap;
		_ambientMap = nullptr;
	}
	_usingAmbientMap = false;
}

// Diffuse map management

void	Material::setDiffuseMap(
	const char* texturePath,
	const bool mipmap,
	const bool repeatS,
	const bool repeatT,
	const GLint minfilter,
	const GLint magfilter
)
{
	clearDiffuseMap();
	_diffuseMap = new Texture(texturePath, TEXTURE_TYPE_DIFFUSE);
	_usingDiffuseMap = true;

	_diffuseMap->setTextureMinFilter(minfilter);
	_diffuseMap->setTextureMagFilter(magfilter);
	if (repeatS)
		_diffuseMap->setTextureWrapS(GL_REPEAT);
	if (repeatT)
		_diffuseMap->setTextureWrapT(GL_REPEAT);
	if (mipmap)
		_diffuseMap->genMipmap();
}

void	Material::useDiffuseMap() const
{
	if (_usingDiffuseMap && _diffuseMap)
	{
		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE);
		_diffuseMap->use();
	}
}

void	Material::clearDiffuseMap()
{
	if (_diffuseMap)
	{
		delete _diffuseMap;
		_diffuseMap = nullptr;
	}
	_usingDiffuseMap = false;
}

// Specular map management

void	Material::setSpecularMap(
	const char* texturePath,
	const bool mipmap,
	const bool repeatS,
	const bool repeatT,
	const GLint minfilter,
	const GLint magfilter
)
{
	clearSpecularMap();
	_specularMap = new Texture(texturePath, TEXTURE_TYPE_SPECULAR);
	_usingSpecularMap = true;

	_specularMap->setTextureMinFilter(minfilter);
	_specularMap->setTextureMagFilter(magfilter);
	if (repeatS)
		_specularMap->setTextureWrapS(GL_REPEAT);
	if (repeatT)
		_specularMap->setTextureWrapT(GL_REPEAT);
	if (mipmap)
		_specularMap->genMipmap();
}

void	Material::useSpecularMap() const
{
	if (_usingSpecularMap && _specularMap)
	{
		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_SPECULAR);
		_specularMap->use();
	}
}

void	Material::clearSpecularMap()
{
	if (_specularMap)
	{
		delete _specularMap;
		_specularMap = nullptr;
	}
	_usingSpecularMap = false;
}

// Emission map management

void	Material::setEmissionMap(
	const char* texturePath,
	const bool mipmap,
	const bool repeatS,
	const bool repeatT,
	const GLint minfilter,
	const GLint magfilter
)
{
	clearEmissionMap();
	_emissionMap = new Texture(texturePath, TEXTURE_TYPE_EMISSION);
	_usingEmissionMap = true;

	_emissionMap->setTextureMinFilter(minfilter);
	_emissionMap->setTextureMagFilter(magfilter);
	if (repeatS)
		_emissionMap->setTextureWrapS(GL_REPEAT);
	if (repeatT)
		_emissionMap->setTextureWrapT(GL_REPEAT);
	if (mipmap)
		_emissionMap->genMipmap();
}

void	Material::useEmissionMap() const
{
	if (_usingEmissionMap && _emissionMap)
	{
		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_EMISSION);
		_emissionMap->use();
	}
}

void	Material::clearEmissionMap()
{
	if (_emissionMap)
	{
		delete _emissionMap;
		_emissionMap = nullptr;
	}
	_usingEmissionMap = false;
}

/* Presets */

void	Material::presetEmerald()
{
	_ambient = TVEC3(0.0215f, 0.1745f, 0.0215f);
	_diffuse = TVEC3(0.07568f, 0.61424f, 0.07568f);
	_specular = TVEC3(0.633f, 0.727811f, 0.633f);
	_shininess = 76.8f;
}

void	Material::presetJade()
{
	_ambient = TVEC3(0.135f, 0.2225f, 0.1575f);
	_diffuse = TVEC3(0.54f, 0.89f, 0.63f);
	_specular = TVEC3(0.316228f, 0.316228f, 0.316228f);
	_shininess = 12.8f;
}

void	Material::presetObsidian()
{
	_ambient = TVEC3(0.05375f, 0.05f, 0.06625f);
	_diffuse = TVEC3(0.18275f, 0.17f, 0.22525f);
	_specular = TVEC3(0.332741f, 0.328634f, 0.346435f);
	_shininess = 38.4f;
}

void	Material::presetPearl()
{
	_ambient = TVEC3(0.25f, 0.20725f, 0.20725f);
	_diffuse = TVEC3(1.0f, 0.829f, 0.829f);
	_specular = TVEC3(0.296648f, 0.296648f, 0.296648f);
	_shininess = 11.264f;
}

void	Material::presetRuby()
{
	_ambient = TVEC3(0.1745f, 0.01175f, 0.01175f);
	_diffuse = TVEC3(0.61424f, 0.04136f, 0.04136f);
	_specular = TVEC3(0.296648f, 0.296648f, 0.296648f);
	_shininess = 76.8f;
}

void	Material::presetTurquoise()
{
	_ambient = TVEC3(0.1f, 0.18725f, 0.1745f);
	_diffuse = TVEC3(0.396f, 0.74151f, 0.69102f);
	_specular = TVEC3(0.297254f, 0.296648f, 0.296648f);
	_shininess = 12.8f;
}

void	Material::presetBrass()
{
	_ambient = TVEC3(0.329412f, 0.223529f, 0.027451f);
	_diffuse = TVEC3(0.780392f, 0.568627f, 0.113725f);
	_specular = TVEC3(0.992157f, 0.941176f, 0.807843f);
	_shininess = 27.89743616f;
}

void	Material::presetBronze()
{
	_ambient = TVEC3(0.2125f, 0.1275f, 0.054f);
	_diffuse = TVEC3(0.714f, 0.4284f, 0.18144f);
	_specular = TVEC3(0.393548f, 0.271906f, 0.166721f);
	_shininess = 25.6f;
}

void	Material::presetChrome()
{
	_ambient = TVEC3(0.25f, 0.25f, 0.25f);
	_diffuse = TVEC3(0.4f, 0.4f, 0.4f);
	_specular = TVEC3(0.774597f, 0.774597f, 0.774597f);
	_shininess = 76.8f;
}

void	Material::presetCopper()
{
	_ambient = TVEC3(0.19125f, 0.0735f, 0.0225f);
	_diffuse = TVEC3(0.7038f, 0.27048f, 0.0828f);
	_specular = TVEC3(0.256777f, 0.137622f, 0.086014f);
	_shininess = 12.8f;
}

void	Material::presetGold()
{
	_ambient = TVEC3(0.24725f, 0.1995f, 0.0745f);
	_diffuse = TVEC3(0.75164f, 0.60648f, 0.22648f);
	_specular = TVEC3(0.628281f, 0.555802f, 0.366065f);
	_shininess = 51.2f;
}

void	Material::presetSilver()
{
	_ambient = TVEC3(0.19225f, 0.19225f, 0.19225f);
	_diffuse = TVEC3(0.50754f, 0.50754f, 0.50754f);
	_specular = TVEC3(0.508273f, 0.508273f, 0.508273f);
	_shininess = 51.2f;
}

void	Material::presetBlackPlastic()
{
	_ambient = TVEC3(0.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.01f, 0.01f, 0.01f);
	_specular = TVEC3(0.50f, 0.50f, 0.50f);
	_shininess = 32.0f;
}

void	Material::presetCyanPlastic()
{
	_ambient = TVEC3(0.0f, 0.1f, 0.06f);
	_diffuse = TVEC3(0.0f, 0.50980392f, 0.50980392f);
	_specular = TVEC3(0.50196078f, 0.50196078f, 0.50196078f);
	_shininess = 32.0f;
}

void	Material::presetGreenPlastic()
{
	_ambient = TVEC3(0.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.1f, 0.35f, 0.1f);
	_specular = TVEC3(0.45f, 0.55f, 0.45f);
	_shininess = 32.0f;
}

void	Material::presetRedPlastic()
{
	_ambient = TVEC3(0.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.5f, 0.0f, 0.0f);
	_specular = TVEC3(0.7f, 0.6f, 0.6f);
	_shininess = 32.0f;
}

void	Material::presetWhitePlastic()
{
	_ambient = TVEC3(0.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.55f, 0.55f, 0.55f);
	_specular = TVEC3(0.70f, 0.70f, 0.70f);
	_shininess = 32.0f;
}

void	Material::presetYellowPlastic()
{
	_ambient = TVEC3(0.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.5f, 0.5f, 0.0f);
	_specular = TVEC3(0.60f, 0.60f, 0.50f);
	_shininess = 32.0f;
}

void	Material::presetBlackRubber()
{
	_ambient = TVEC3(0.02f, 0.02f, 0.02f);
	_diffuse = TVEC3(0.01f, 0.01f, 0.01f);
	_specular = TVEC3(0.4f, 0.4f, 0.4f);
	_shininess = 10.0f;
}

void	Material::presetCyanRubber()
{
	_ambient = TVEC3(0.0f, 0.05f, 0.05f);
	_diffuse = TVEC3(0.4f, 0.5f, 0.5f);
	_specular = TVEC3(0.04f, 0.7f, 0.7f);
	_shininess = 10.0f;
}

void	Material::presetGreenRubber()
{
	_ambient = TVEC3(0.0f, 0.05f, 0.0f);
	_diffuse = TVEC3(0.4f, 0.5f, 0.4f);
	_specular = TVEC3(0.04f, 0.7f, 0.04f);
	_shininess = 10.0f;
}

void	Material::presetRedRubber()
{
	_ambient = TVEC3(0.05f, 0.0f, 0.0f);
	_diffuse = TVEC3(0.5f, 0.4f, 0.4f);
	_specular = TVEC3(0.7f, 0.04f, 0.04f);
	_shininess = 10.0f;
}

void	Material::presetWhiteRubber()
{
	_ambient = TVEC3(0.05f, 0.05f, 0.05f);
	_diffuse = TVEC3(0.5f, 0.5f, 0.5f);
	_specular = TVEC3(0.7f, 0.7f, 0.7f);
	_shininess = 10.0f;
}

void	Material::presetYellowRubber()
{
	_ambient = TVEC3(0.05f, 0.05f, 0.0f);
	_diffuse = TVEC3(0.5f, 0.5f, 0.4f);
	_specular = TVEC3(0.7f, 0.7f, 0.04f);
	_shininess = 10.0f;
}

void	Material::presetBianca()
{
	_ambient = TVEC3(0.18f, 0.102f, 0.086f);
	_diffuse = TVEC3(0.639f, 0.412f, 0.173f);
	_specular = TVEC3(0.678f, 0.435f, 0.176f);
	_shininess = 4.0f;
}

void	Material::presetWhiteLight()
{
	_ambient = TVEC3(1.0f, 1.0f, 1.0f);
	_diffuse = TVEC3(1.0f, 1.0f, 1.0f);
	_specular = TVEC3(1.0f, 1.0f, 1.0f);
	_emission = TVEC3(1.0f, 1.0f, 1.0f);
	_shininess = 2.0f;
}

void	Material::presetRedLight()
{
	_ambient = TVEC3(1.0f, 0.0f, 0.0f);
	_diffuse = TVEC3(1.0f, 0.0f, 0.0f);
	_specular = TVEC3(1.0f, 0.0f, 0.0f);
	_emission = TVEC3(1.0f, 0.0f, 0.0f);
	_shininess = 2.0f;
}

void	Material::presetGreenLight()
{
	_ambient = TVEC3(0.0f, 1.0f, 0.0f);
	_diffuse = TVEC3(0.0f, 1.0f, 0.0f);
	_specular = TVEC3(0.0f, 1.0f, 0.0f);
	_emission = TVEC3(0.0f, 1.0f, 0.0f);
	_shininess = 2.0f;
}

void	Material::presetBlueLight()
{
	_ambient = TVEC3(0.0f, 0.0f, 1.0f);
	_diffuse = TVEC3(0.0f, 0.0f, 1.0f);
	_specular = TVEC3(0.0f, 0.0f, 1.0f);
	_emission = TVEC3(0.0f, 0.0f, 1.0f);
	_shininess = 2.0f;
}