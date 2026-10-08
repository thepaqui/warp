/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spotlight.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 23:12:43 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 15:15:28 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spotlight.hpp"

Spotlight::Spotlight(const Spotlight &obj)
{
	_direction = obj.getDirection();
	_position = obj.getPos();
	_innerCutOff = obj.getInnerCutOff();
	_outerCutOff = obj.getOuterCutOff();
	_color = obj.getColor();
	_colorConvert = obj.getColorConvert();
	_ambient = obj.getAmbientIntensity();
	_diffuse = obj.getDiffuseIntensity();
	_specular = obj.getSpecularIntensity();
	_attenuation_constant = obj.getAttenuationConstant();
	_attenuation_linear = obj.getAttenuationLinear();
	_attenuation_quadratic = obj.getAttenuationQuadratic();
}

Spotlight	&Spotlight::operator=(const Spotlight &obj)
{
	if (this != &obj)
	{
		_direction = obj.getDirection();
		_position = obj.getPos();
		_innerCutOff = obj.getInnerCutOff();
		_outerCutOff = obj.getOuterCutOff();
		_color = obj.getColor();
		_colorConvert = obj.getColorConvert();
		_ambient = obj.getAmbientIntensity();
		_diffuse = obj.getDiffuseIntensity();
		_specular = obj.getSpecularIntensity();
		_attenuation_constant = obj.getAttenuationConstant();
		_attenuation_linear = obj.getAttenuationLinear();
		_attenuation_quadratic = obj.getAttenuationQuadratic();
	}
	return (*this);
}

// Display color is multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	Spotlight::convertColor()
{
	_ambient = _color * _colorConvert.getElem(0);
	_diffuse = _color * _colorConvert.getElem(1);
	_specular = _color * _colorConvert.getElem(2);
	_changed = true;
}

// Changes display color to specified one
// Display color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	Spotlight::convertColor(const MATF &newColorVec3)
{
	setColor(newColorVec3);
	convertColor();
}

// Changes display color and colorConvert to specified ones
// Display color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	Spotlight::convertColor(const MATF &newColorVec3,
	const MATF &newColorConvertVec3)
{
	setColor(newColorVec3);
	setColorConvert(newColorConvertVec3);
	convertColor();
}

void	Spotlight::sendProperties(ShaderProgram &sp, const std::string &prefix)
{
	sp.setFloatVec3(prefix + "position", _position);
	sp.setFloatVec3(prefix + "direction", _direction);
	if (_active)
	{
		sp.setFloatVec3(prefix + "ambientIntensity", _ambient);
		sp.setFloatVec3(prefix + "diffuseIntensity", _diffuse);
		sp.setFloatVec3(prefix + "specularIntensity", _specular);
	}
	else
	{
		sp.setFloatVec3(prefix + "ambientIntensity", TVEC3(0.0f, 0.0f, 0.0f));
		sp.setFloatVec3(prefix + "diffuseIntensity", TVEC3(0.0f, 0.0f, 0.0f));
		sp.setFloatVec3(prefix + "specularIntensity", TVEC3(0.0f, 0.0f, 0.0f));
	}
	sp.setFloat(prefix + "attenuationConstant", _attenuation_constant);
	sp.setFloat(prefix + "attenuationLinear", _attenuation_linear);
	sp.setFloat(prefix + "attenuationQuadratic", _attenuation_quadratic);
	sp.setFloat(prefix + "innerCutOff", cos(Matrix<float>::degToRad(_innerCutOff)));
	sp.setFloat(prefix + "outerCutOff", cos(Matrix<float>::degToRad(_outerCutOff)));
	_changed = false;
}

void	Spotlight::sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix)
{
	if (!doesIlluminationModelUseLights(illumModel))
		return ;

	sp.setFloatVec3(prefix + "position", _position);
	sp.setFloatVec3(prefix + "direction", _direction);
	if (doesIlluminationModelUseAmbient(illumModel))
	{
		auto a = _active ? _ambient : TVEC3(0.0f, 0.0f, 0.0f);
		sp.setFloatVec3(prefix + "ambientIntensity", a);
	}
	if (doesIlluminationModelUseDiffuse(illumModel))
	{
		auto d = _active ? _diffuse : TVEC3(0.0f, 0.0f, 0.0f);
		sp.setFloatVec3(prefix + "diffuseIntensity", d);
	}
	if (doesIlluminationModelUseSpecular(illumModel))
	{
		auto s = _active ? _specular : TVEC3(0.0f, 0.0f, 0.0f);
		sp.setFloatVec3(prefix + "specularIntensity", s);
	}
	sp.setFloat(prefix + "attenuationConstant", _attenuation_constant);
	sp.setFloat(prefix + "attenuationLinear", _attenuation_linear);
	sp.setFloat(prefix + "attenuationQuadratic", _attenuation_quadratic);
	sp.setFloat(prefix + "innerCutOff", cos(Matrix<float>::degToRad(_innerCutOff)));
	sp.setFloat(prefix + "outerCutOff", cos(Matrix<float>::degToRad(_outerCutOff)));
	_changed = false;
}

/* Attenuation Presets */

void	Spotlight::attenuationPreset7() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.7f;
	_attenuation_quadratic = 1.8f;
	_changed = true;
}

void	Spotlight::attenuationPreset13() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.35f;
	_attenuation_quadratic = 0.44f;
	_changed = true;
}

void	Spotlight::attenuationPreset20() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.22f;
	_attenuation_quadratic = 0.2f;
	_changed = true;
}

void	Spotlight::attenuationPreset32() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.14f;
	_attenuation_quadratic = 0.07f;
	_changed = true;
}

void	Spotlight::attenuationPreset50() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.09f;
	_attenuation_quadratic = 0.032f;
	_changed = true;
}

void	Spotlight::attenuationPreset65() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.07f;
	_attenuation_quadratic = 0.017f;
	_changed = true;
}

void	Spotlight::attenuationPreset100() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.045f;
	_attenuation_quadratic = 0.0075f;
	_changed = true;
}

void	Spotlight::attenuationPreset160() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.027f;
	_attenuation_quadratic = 0.0028f;
	_changed = true;
}

void	Spotlight::attenuationPreset200() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.022f;
	_attenuation_quadratic = 0.0019f;
	_changed = true;
}

void	Spotlight::attenuationPreset325() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.014f;
	_attenuation_quadratic = 0.0007f;
	_changed = true;
}

void	Spotlight::attenuationPreset600() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.007f;
	_attenuation_quadratic = 0.0002f;
	_changed = true;
}

void	Spotlight::attenuationPreset3250() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.0014f;
	_attenuation_quadratic = 0.000007f;
	_changed = true;
}
