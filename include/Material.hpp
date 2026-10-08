/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 21:37:16 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/03 16:01:31 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_HPP
# define MATERIAL_HPP
# include "glad/glad.hpp"

class Material;

# include "Transform.hpp"
# include "Texture2D.hpp"
# include "IlluminationModels.hpp"
# include "ShaderInfo.hpp"
# include "ShaderProgram.hpp"
# include "ShaderManager.hpp"

// Default values:
// - ambient color (0.1, 0.1, 0.1)
// - diffuse color (0.8, 0.8, 0.8)
// - specular color (1, 1, 1)
// - emission color (0, 0, 0)
// - transmission filter (1, 1, 1)
// - shininess = 2
// - opacity = 1 (fully opaque)
// - refractive index = 1 (air)
// - illumination model = 2 (Blinn-Phong)
class Material
{
private	:
	MATF	_ambient = TVEC3(0.1f, 0.1f, 0.1f);
	MATF	_diffuse = TVEC3(0.8f, 0.8f, 0.8f);
	MATF	_specular = TVEC3(1.0f, 1.0f, 1.0f);
	MATF	_emission = TVEC3(0.0f, 0.0f, 0.0f);

	// TODO: is this even used?
	// Used to filter light passing through transparent materials
	// Default is white (no filtering)
	MATF	_transmissionFilter = TVEC3(1.0f, 1.0f, 1.0f);

	// The shininess of a surface determines
	// the size of the specular light's "halo" on it
	// The higher the value, the smaller the halo is
	// Negative values are very weird but not an error
	float	_shininess = 2.0f;

	// 1.0 = fully opaque, 0.0 = fully transparent
	// Clamped to [0.0, 1.0]
	float	_opacity = 1.0f;

	// TODO: is this even used?
	// Refractive index / Optical Density
	// Controls how light bends when entering the material
	// Only used when opacity < 1.0
	float	_refractiveIndex = 1.0f;

	IlluminationModel	_illuminationModel = ILLUM_MODEL_HIGHLIGHT_ON;

	std::optional<size_t>	_customShaderID = std::nullopt;

	// Texture for ambient map, nullptr if not using one
	Texture*	_ambientMap = nullptr;
	bool		_usingAmbientMap = false;

	// Texture for diffuse map, nullptr if not using one
	Texture*	_diffuseMap = nullptr;
	bool		_usingDiffuseMap = false;

	// Texture for specular map, nullptr if not using one
	Texture*	_specularMap = nullptr;
	bool		_usingSpecularMap = false;

	// Texture for emission map, nullptr if not using one
	Texture*	_emissionMap = nullptr;
	bool		_usingEmissionMap = false;

public	:
	Material() {};
	Material(const Material &obj);
	// Move semantics
	Material(Material&& other) noexcept;
	Material& operator=(Material&& other) noexcept;
	~Material();

	Material	&operator=(const Material &obj) noexcept;

	void	printInfo() const;

	// TODO: remove this overload
	void	apply(ShaderProgram &shader) const;
	void	apply(ShaderProgram& shader, const ShaderInfo &shaderInfo) const;
	void	apply(const ShaderManager& shaderManager, const ShaderInfo &shaderInfo) const;

	IlluminationModel		getIlluminationModel() const noexcept { return _illuminationModel; };
	std::optional<size_t>	getCustomShaderID() const noexcept { return _customShaderID; };
	ShaderInfo	getShaderInfo() const;

	const MATF	&getAmbientColor() const noexcept { return _ambient; };
	const MATF	&getDiffuseColor() const noexcept { return _diffuse; };
	const MATF	&getSpecularColor() const noexcept { return _specular; };
	const MATF	&getEmissionColor() const noexcept { return _emission; };
	const MATF	&getTransmissionFilter() const noexcept { return _transmissionFilter; };
	float		getShininess() const noexcept { return _shininess; };
	float		getOpacity() const noexcept { return _opacity; };
	float		getRefractiveIndex() const noexcept { return _refractiveIndex; };
	bool		isUsingAmbientMap() const noexcept { return _usingAmbientMap; };
	bool		isUsingDiffuseMap() const noexcept { return _usingDiffuseMap; };
	bool		isUsingSpecularMap() const noexcept { return _usingSpecularMap; };
	bool		isUsingEmmissionMap() const noexcept { return _usingEmissionMap; };

	void	setIlluminationModel(const IlluminationModel model) noexcept { _illuminationModel = model; };
	void	setIlluminationModel(const int model) noexcept { _illuminationModel = static_cast<IlluminationModel>(model); };
	void	setCustomShaderID(const size_t id) noexcept { _customShaderID = id; };
	void	clearCustomShaderID() noexcept { _customShaderID = std::nullopt; };

	void	setAmbientColor(const MATF &vec3) { _ambient = vec3; };
	void	setDiffuseColor(const MATF &vec3) { _diffuse = vec3; };
	void	setSpecularColor(const MATF &vec3) { _specular = vec3; };
	void	setEmissionColor(const MATF &vec3) { _emission = vec3; };
	void	setTransmissionFilter(const MATF &vec3) { _transmissionFilter = vec3; };
	void	setShininess(const float value) noexcept { _shininess = value; };
	void	setOpacity(const float value) noexcept { _opacity = std::clamp(value, 0.0f, 1.0f); };
	void	setRefractiveIndex(const float value) noexcept { _refractiveIndex = value; };

	// Ambient map management
	void	setAmbientMap(
		const char* texturePath,
		const bool mipmap = false,
		const bool repeatS = false,
		const bool repeatT = false,
		const GLint minfilter = GL_NEAREST,
		const GLint magfilter = GL_NEAREST
	);
	void	useAmbientMap() const;
	void	clearAmbientMap();

	// Diffuse map management
	void	setDiffuseMap(
		const char* texturePath,
		const bool mipmap = false,
		const bool repeatS = false,
		const bool repeatT = false,
		const GLint minfilter = GL_NEAREST,
		const GLint magfilter = GL_NEAREST
	);
	void	useDiffuseMap() const;
	void	clearDiffuseMap();

	// Specular map management
	void	setSpecularMap(
		const char* texturePath,
		const bool mipmap = false,
		const bool repeatS = false,
		const bool repeatT = false,
		const GLint minfilter = GL_NEAREST,
		const GLint magfilter = GL_NEAREST
	);
	void	useSpecularMap() const;
	void	clearSpecularMap();

	// Emission map management
	void	setEmissionMap(
		const char* texturePath,
		const bool mipmap = false,
		const bool repeatS = false,
		const bool repeatT = false,
		const GLint minfilter = GL_NEAREST,
		const GLint magfilter = GL_NEAREST
	);
	void	useEmissionMap() const;
	void	clearEmissionMap();

	/* Presets */
	// These are taken from
	// http://devernay.free.fr/cours/opengl/materials.html

	void	presetEmerald();
	void	presetJade();
	void	presetObsidian();
	void	presetPearl();
	void	presetRuby();
	void	presetTurquoise();
	void	presetBrass();
	void	presetBronze();
	void	presetChrome();
	void	presetCopper();
	void	presetGold();
	void	presetSilver();
	void	presetBlackPlastic();
	void	presetCyanPlastic();
	void	presetGreenPlastic();
	void	presetRedPlastic();
	void	presetWhitePlastic();
	void	presetYellowPlastic();
	void	presetBlackRubber();
	void	presetCyanRubber();
	void	presetGreenRubber();
	void	presetRedRubber();
	void	presetWhiteRubber();
	void	presetYellowRubber();

	// My own presets

	void	presetBianca();
	void	presetWhiteLight();
	void	presetRedLight();
	void	presetGreenLight();
	void	presetBlueLight();
};

#endif