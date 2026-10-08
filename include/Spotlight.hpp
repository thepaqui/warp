/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spotlight.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:55:38 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 16:11:56 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPOTLIGHT_HPP
# define SPOTLIGHT_HPP
# include "ShaderProgram.hpp"
# include "Transform.hpp"
# include "IlluminationModels.hpp"
# include "Object.hpp"

// Default values:
// - direction (0, 0, -1)
// - position (0, 0, 0)
// - innerCutOff = 12.5 (degrees)
// - outerCutOff = 17.5 (degrees)
// - colorConvert (0.1, 0.5, 1)
//   see convertColor() for more info on this data
// - ambient intensity (0.1, 0.1, 0.1)
// - diffuse intensity (0.5, 0.5, 0.5)
// - specular intensity (1, 1, 1)
class Spotlight
{
private	:
	Transform	_mat;
	bool		_active = true;

	MATF	_direction = TVEC3(0.0f, 0.0f, -1.0f);
	MATF	_position = TVEC3(0.0f, 0.0f, 0.0f);
	float	_innerCutOff = 12.5f; // degrees
	float	_outerCutOff = 17.5f; // degrees

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
	bool	_changed = true;

	void	setColor(const MATF &vec3) { _color = vec3; };
	void	setColorConvert(const MATF &vec3) { _colorConvert = vec3; };
	void	convertColor();

	void	sendProperties(ShaderProgram &sp, const std::string &prefix);
	void	sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix);

protected	:
	const float	&getInnerCutOff() const noexcept { return _innerCutOff; };
	const float	&getOuterCutOff() const noexcept { return _outerCutOff; };
	const MATF	&getColorConvert() const noexcept { return _colorConvert; };
	const MATF	&getAmbientIntensity() const noexcept { return _ambient; };
	const MATF	&getDiffuseIntensity() const noexcept { return _diffuse; };
	const MATF	&getSpecularIntensity() const noexcept { return _specular; };
	const float	&getAttenuationConstant() const noexcept { return _attenuation_constant; };
	const float	&getAttenuationLinear() const noexcept { return _attenuation_linear; };
	const float	&getAttenuationQuadratic() const noexcept { return _attenuation_quadratic; };

public	:
	Spotlight() {};
	Spotlight(const Spotlight &obj);
	~Spotlight() {};

	Spotlight	&operator=(const Spotlight &obj);

	bool		isActive() const noexcept { return _active; };
	const MATF	&getPos() const noexcept { return _position; };
	const MATF	&getDirection() const noexcept { return _direction; };
	const MATF	&getColor() const noexcept { return _color; };

	void	setActive(const bool active) noexcept { _active = active; };
	void	activate() noexcept { _active = true; };
	void	deactivate() noexcept { _active = false; };
	void	toggleActive() noexcept { _active = !_active; };

	void	setDirection(const MATF &vec3) { _direction = vec3; _changed = true; };
	void	setPos(const MATF &vec3) { _position = vec3; _changed = true; };
	void	setInnerCutOff(const float angleInDegrees) noexcept { _innerCutOff = angleInDegrees; _changed = true; };
	void	setOuterCutOff(const float angleInDegrees) noexcept { _outerCutOff = angleInDegrees; _changed = true; };
	void	setAmbientIntensity(const MATF &vec3) { _ambient = vec3; _changed = true; };
	void	setDiffuseIntensity(const MATF &vec3) { _diffuse = vec3; _changed = true; };
	void	setSpecularIntensity(const MATF &vec3) { _specular = vec3; _changed = true; };
	void	setIntensity(
		const MATF &ambientVec3,
		const MATF &diffuseVec3,
		const MATF &specularVec3
	)
	{
		_ambient = ambientVec3;
		_diffuse = diffuseVec3;
		_specular = specularVec3;
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

	void	lookAt(const MATF &target)
	{
		MATF direction = target - _position;
		direction = Matrix<float>::normalize(direction);
		setDirection(direction);
	};

	void	lookAt(const Object& obj)
	{
		lookAt(obj.getPos());
	};

	void	convertColor(const MATF &newColorVec3);
	void	convertColor(const MATF &newColorVec3, const MATF &newColorConvertVec3);

	void	sendProperties(ShaderProgram &shader)
	{ sendProperties(shader, "spotLight."); }
	void	sendProperties(ShaderProgram &shader, const IlluminationModel illumModel, unsigned int index)
	{
		const std::string prefix = "spotLight[" + std::to_string(index) + "].";
		sendProperties(shader, illumModel, prefix);
	};

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
