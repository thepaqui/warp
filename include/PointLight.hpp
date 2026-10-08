/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointLight.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:55:38 by thepaqui          #+#    #+#             */
/*   Updated: 2026/03/06 18:00:20 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_LIGHT_HPP
# define POINT_LIGHT_HPP
# include "Model.hpp"
# include "ShaderProgram.hpp"
# include "Transform.hpp"

// Default values:
// - scale (0.1, 0.1, 0.1)
// - rotation (0, 0, 0)
// - position (0, 0, 0)
// - visible = true
// - color (1, 1, 1)
//   Display color of the light source
// - colorConvert (0.1, 0.5, 1)
//   see convertColor() for more info on this data
// - ambient intensity (0.1, 0.1, 0.1)
// - diffuse intensity (0.5, 0.5, 0.5)
// - specular intensity (1, 1, 1)
class PointLight
{
private	:
	Model*		_model = NULL;
	Transform	_mat;
	bool		_active = true;

	MATF	_scale = TVEC3(0.1f, 0.1f, 0.1f);
	MATF	_rotation = TVEC3(0.0f, 0.0f, 0.0f);
	MATF	_position = TVEC3(0.0f, 0.0f, 0.0f);
	bool	_visible = true;

	MATF	_color = TVEC3(1.0f, 1.0f, 1.0f);
	MATF	_colorConvert = TVEC3(0.1f, 0.5f, 1.0f);

	MATF	_ambient = TVEC3(0.1f, 0.1f, 0.1f);
	MATF	_diffuse = TVEC3(0.5f, 0.5f, 0.5f);
	MATF	_specular = TVEC3(1.0f, 1.0f, 1.0f);

	// Attenuation factors
	// Default values for a distance of 50 units
	
	float	_attenuation_constant = 1.0f;
	float	_attenuation_linear = 0.09f;
	float	_attenuation_quadratic = 0.032f;

	// Indicates if light properties have changed since last send to shader
	// Not for model matrix changes
	bool	_changed = true;

	void	setColor(const MATF &vec3) { _color = vec3; };
	void	setColorConvert(const MATF &vec3) { _colorConvert = vec3; };
	void	convertColor();

	void	sendProperties(ShaderProgram &sp, const std::string &prefix);
	void	sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix);
	void	sendModelMatrix(ShaderProgram &sp);
	void	sendModelMatrix(ShaderProgram &sp, const ShaderInfo& shaderInfo);

protected	:
	const Model	*getModel() const noexcept { return _model; };
	const MATF	&getScale() const noexcept { return _scale; };
	const MATF	&getRot() const noexcept { return _rotation; };
	bool		getVisibility() const noexcept { return _visible; };
	const MATF	&getColorConvert() const noexcept { return _colorConvert; };
	const MATF	&getAmbientIntensity() const noexcept { return _ambient; };
	const MATF	&getDiffuseIntensity() const noexcept { return _diffuse; };
	const MATF	&getSpecularIntensity() const noexcept { return _specular; };
	const float	&getAttenuationConstant() const noexcept { return _attenuation_constant; };
	const float	&getAttenuationLinear() const noexcept { return _attenuation_linear; };
	const float	&getAttenuationQuadratic() const noexcept { return _attenuation_quadratic; };

public	:
	PointLight() {};
	PointLight(Model*const model) { _model = model; };
	PointLight(const PointLight &obj);
	~PointLight() {};

	PointLight	&operator=(const PointLight &obj);

	bool		isActive() const noexcept { return _active; };
	const MATF	&getPos() const noexcept { return _position; };
	const MATF	&getColor() const noexcept { return _color; };

	std::vector<ShaderInfo>	getUsedShadersInfo() const
	{
		if (!_model || !_visible)
			return {};
		return _model->getUsedShadersInfo();
	};

	void	setActive(const bool active) noexcept { _active = active; };
	void	activate() noexcept { _active = true; };
	void	deactivate() noexcept { _active = false; };
	void	toggleActive() noexcept { _active = !_active; };

	void	setScale(const MATF &vec3) { _scale = vec3; };
	void	setRot(const MATF &vec3) { _rotation = vec3; };
	void	setPos(const MATF &vec3) { _position = vec3; _changed = true; };
	void	setAmbientIntensity(const MATF &vec3) { _ambient = vec3; _changed = true; };
	void	setDiffuseIntensity(const MATF &vec3) { _diffuse = vec3; _changed = true; };
	void	setSpecularIntensity(const MATF &vec3) { _specular = vec3; _changed = true; };
	void	setIntensity(
		const MATF &ambient,
		const MATF &diffuse,
		const MATF &specular
	)
	{
		_ambient = ambient;
		_diffuse = diffuse;
		_specular = specular;
		_changed = true;
	};
	void	setAttenuationConstant(const float value) noexcept { _attenuation_constant = value; _changed = true; };
	void	setAttenuationLinear(const float value) noexcept { _attenuation_linear = value; _changed = true; };
	void	setAttenuationQuadratic(const float value) noexcept { _attenuation_quadratic = value; _changed = true; };
	void	setAttenuation(
		const float constant,
		const float linear,
		const float quadratic
	) noexcept
	{
		_attenuation_constant = constant;
		_attenuation_linear = linear;
		_attenuation_quadratic = quadratic;
		_changed = true;
	};

	void	convertColor(const MATF &newColorVec3);
	void	convertColor(const MATF &newColorVec3, const MATF &newColorConvertVec3);

	void	sendProperties(ShaderProgram &shader)
	{
		sendProperties(shader, "pointLight.");
	};

	void	sendProperties(ShaderProgram &shader, const IlluminationModel illumModel, unsigned int index)
	{
		const std::string prefix = "pointLight[" + std::to_string(index) + "].";
		sendProperties(shader, illumModel, prefix);
	};

	void	resize(const MATF &vec3);
	void	resizeX(const float x, bool keepRatio);
	void	resizeY(const float y, bool keepRatio);
	void	resizeZ(const float z, bool keepRatio);
	void	resizeBiggestDimension(const float maxDimension);

	void	forceMaterial(const std::string &materialName, Material&& material);

// TODO : delete
	void	draw(ShaderProgram &shader)
	{
		sendProperties(shader, "pointLight.");
		if (!_visible || !_model)
			return ;
		sendModelMatrix(shader);
		_model->draw(shader);
	};

	void	drawOnly(ShaderProgram &shader, const ShaderInfo& shaderInfo)
	{
		if (!_visible || !_model)
			return ;
		sendModelMatrix(shader, shaderInfo);
		_model->drawOnly(shader, shaderInfo);
	};

	void	makeVisible() noexcept { _visible = true; };
	void	makeInvisible() noexcept { _visible = false; };
	void	toggleVisibility() noexcept { _visible = !_visible; };

	// Sets visibility and active state to true
	void	use() noexcept { _visible = true; _active = true; };
	// Sets visibility and active state to false
	void	stop() noexcept { _visible = false; _active = false; };

	/* Attenuation Presets */
	void	attenuationPreset7() noexcept;
	void	attenuationPreset13() noexcept;
	void	attenuationPreset20() noexcept;
	void	attenuationPreset32() noexcept;
	void	attenuationPreset50() noexcept;
	void	attenuationPreset65() noexcept;
	void	attenuationPreset100() noexcept;
	void	attenuationPreset160() noexcept;
	void	attenuationPreset200() noexcept;
	void	attenuationPreset325() noexcept;
	void	attenuationPreset600() noexcept;
	void	attenuationPreset3250() noexcept;
};

#endif
