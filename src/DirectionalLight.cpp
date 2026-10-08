/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalLight.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 23:12:43 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 14:22:30 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(const MATF &direction)
{
	_direction = direction;
}

DirectionalLight::DirectionalLight(const DirectionalLight &obj)
{
	_direction = obj.getDirection();
	_ambient = obj.getAmbientIntensity();
	_diffuse = obj.getDiffuseIntensity();
	_specular = obj.getSpecularIntensity();
}

DirectionalLight	&DirectionalLight::operator=(const DirectionalLight &obj)
{
	if (this != &obj)
	{
		_direction = obj.getDirection();
		_ambient = obj.getAmbientIntensity();
		_diffuse = obj.getDiffuseIntensity();
		_specular = obj.getSpecularIntensity();
	}
	return (*this);
}

// Color is multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	DirectionalLight::convertColor()
{
	_ambient = _color * _colorConvert.getElem(0);
	_diffuse = _color * _colorConvert.getElem(1);
	_specular = _color * _colorConvert.getElem(2);
	_changed = true;
}

// Changes color to specified one
// Color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	DirectionalLight::convertColor(const MATF &newColorVec3)
{
	setColor(newColorVec3);
	convertColor();
}

// Changes color and colorConvert to specified ones
// Color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	DirectionalLight::convertColor(const MATF &newColorVec3,
	const MATF &newColorConvertVec3)
{
	setColor(newColorVec3);
	setColorConvert(newColorConvertVec3);
	convertColor();
}

void	DirectionalLight::sendProperties(ShaderProgram &sp, const std::string &prefix)
{
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
	sp.setFloatVec3(prefix + "direction", _direction);
	_changed = false;
}

void	DirectionalLight::sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix)
{
	if (!doesIlluminationModelUseLights(illumModel))
		return ;
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
	sp.setFloatVec3(prefix + "direction", _direction);
	_changed = false;
}
