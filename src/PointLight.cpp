/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointLight.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 23:12:43 by thepaqui          #+#    #+#             */
/*   Updated: 2026/03/06 17:30:28 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PointLight.hpp"

PointLight::PointLight(const PointLight &obj)
{
	_model = obj._model;
	_scale = obj.getScale();
	_rotation = obj.getRot();
	_position = obj.getPos();
	_visible = obj.getVisibility();
	_color = obj.getColor();
	_colorConvert = obj.getColorConvert();
	_ambient = obj.getAmbientIntensity();
	_diffuse = obj.getDiffuseIntensity();
	_specular = obj.getSpecularIntensity();
	_attenuation_constant = obj.getAttenuationConstant();
	_attenuation_linear = obj.getAttenuationLinear();
	_attenuation_quadratic = obj.getAttenuationQuadratic();
}

PointLight	&PointLight::operator=(const PointLight &obj)
{
	if (this != &obj)
	{
		_model = obj._model;
		_scale = obj.getScale();
		_rotation = obj.getRot();
		_position = obj.getPos();
		_visible = obj.getVisibility();
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
void	PointLight::convertColor()
{
	_ambient = _color * _colorConvert.getElem(0);
	_diffuse = _color * _colorConvert.getElem(1);
	_specular = _color * _colorConvert.getElem(2);
	_changed = true;
}

// Changes display color to specified one
// Display color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	PointLight::convertColor(const MATF &newColorVec3)
{
	setColor(newColorVec3);
	convertColor();
}

// Changes display color and colorConvert to specified ones
// Display color is then multiplied with each component of colorConvert
// to get new ambient, diffuse and specular values respectively
void	PointLight::convertColor(const MATF &newColorVec3,
	const MATF &newColorConvertVec3)
{
	setColor(newColorVec3);
	setColorConvert(newColorConvertVec3);
	convertColor();
}

// TODO : delete ?
void	PointLight::sendModelMatrix(ShaderProgram &sp)
{
	if (!_visible || !_model)
	{
		sp.setFloatMat4("model", MATF(4, 4, Mat_null).getData());
		return ;
	}
	_mat.reset();
	_mat.scale(_scale);
	_mat.rotateX(_rotation.getElem(0));
	_mat.rotateY(_rotation.getElem(1));
	_mat.rotateZ(_rotation.getElem(2));
	_mat.translate(_position);
	sp.setFloatMat4("model", _mat.data());
}

void	PointLight::sendModelMatrix(ShaderProgram &sp, const ShaderInfo& shaderInfo)
{
	if (!_visible || !_model)
	{
		sp.setFloatMat4("model", MATF(4, 4, Mat_null).getData());
		return ;
	}
	_mat.reset();
	_mat.scale(_scale);
	_mat.rotateX(_rotation.getElem(0));
	_mat.rotateY(_rotation.getElem(1));
	_mat.rotateZ(_rotation.getElem(2));
	_mat.translate(_position);
	sp.setFloatMat4("model", _mat.data());

	if (doesIlluminationModelUseNormalMatrix(shaderInfo.getIlluminationModel()))
	{
		MATF normalMatrix = _mat.getNormalMatrix();
		sp.setFloatMat3("normalMatrix", normalMatrix.getData());
	}
}

void	PointLight::sendProperties(ShaderProgram &sp, const std::string &prefix)
{
	sp.setFloatVec3(prefix + "position", _position);
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
	_changed = false;
}

void	PointLight::sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix)
{
	if (!doesIlluminationModelUseLights(illumModel))
		return ;

	sp.setFloatVec3(prefix + "position", _position);
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
	_changed = false;
}

// Adjusts the object's scale in order make its size match the given values
// If the underlying model has one or more sizes of 0, those dimensions are ignored
// If the model doesn't exist, nothing happens
void	PointLight::resize(const MATF &vec3)
{
	if (!_model)
		return ;
	const MATF	modelSize = _model->getSize();
	if (modelSize.getElem(0) != 0)
		_scale.setElem(0, vec3.getElem(0) / modelSize.getElem(0));
	if (modelSize.getElem(1) != 0)
		_scale.setElem(1, vec3.getElem(1) / modelSize.getElem(1));
	if (modelSize.getElem(2) != 0)
		_scale.setElem(2, vec3.getElem(2) / modelSize.getElem(2));
}

// Resizes the object to the given X size value
// If keepRatio is true, Y and Z scales are adjusted to maintain proportions
// If the model does not exist or its X size is 0, nothing happens
// If the object's current X scale is 0, keepRatio is ignored
void	PointLight::resizeX(const float x, bool keepRatio)
{
	if (!_model)
		return ;

	const MATF	modelSize = _model->getSize();
	if (modelSize.getElem(0) == 0)
		return;

	const float	scaleFactor = x / modelSize.getElem(0);
	const float	oldScaleX = _scale.getElem(0);
	_scale.setElem(0, scaleFactor);

	if (keepRatio && oldScaleX != 0)
	{
		const float	scaleScaleFactor = scaleFactor / oldScaleX;
		_scale.setElem(1, _scale.getElem(1) * scaleScaleFactor);
		_scale.setElem(2, _scale.getElem(2) * scaleScaleFactor);
	}
}

// Resizes the object to the given Y size value
// If keepRatio is true, X and Z scales are adjusted to maintain proportions
// If the model does not exist or its Y size is 0, nothing happens
// If the object's current Y scale is 0, keepRatio is ignored
void	PointLight::resizeY(const float y, bool keepRatio)
{
	if (!_model)
		return ;

	const MATF	modelSize = _model->getSize();
	if (modelSize.getElem(1) == 0)
		return;

	const float	scaleFactor = y / modelSize.getElem(1);
	const float	oldScaleY = _scale.getElem(1);
	_scale.setElem(1, scaleFactor);

	if (keepRatio && oldScaleY != 0)
	{
		const float	scaleScaleFactor = scaleFactor / oldScaleY;
		_scale.setElem(0, _scale.getElem(0) * scaleScaleFactor);
		_scale.setElem(2, _scale.getElem(2) * scaleScaleFactor);
	}
}

// Resizes the object to the given Z size value
// If keepRatio is true, X and Y scales are adjusted to maintain proportions
// If the model does not exist or its Z size is 0, nothing happens
// If the object's current Z scale is 0, keepRatio is ignored
void	PointLight::resizeZ(const float z, bool keepRatio)
{
	if (!_model)
		return ;

	const MATF	modelSize = _model->getSize();
	if (modelSize.getElem(2) == 0)
		return;

	const float	scaleFactor = z / modelSize.getElem(2);
	const float	oldScaleZ = _scale.getElem(2);
	_scale.setElem(2, scaleFactor);

	if (keepRatio && oldScaleZ != 0)
	{
		const float	scaleScaleFactor = scaleFactor / oldScaleZ;
		_scale.setElem(0, _scale.getElem(0) * scaleScaleFactor);
		_scale.setElem(1, _scale.getElem(1) * scaleScaleFactor);
	}
}

// Resizes the object so its model's biggest dimension matches the provided value
// Of course, this keeps the previous scale ratio intact
// If the object has no model or the model has an all 0 size, nothing happens
void	PointLight::resizeBiggestDimension(const float maxDimension)
{
	if (!_model)
		return ;

	const MATF	modelSize = _model->getSize();
	uint8_t		biggestDimension = 0;
	float		currentMaxDimension = modelSize.getElem(0);
	if (modelSize.getElem(1) > currentMaxDimension)
	{
		currentMaxDimension = modelSize.getElem(1);
		biggestDimension = 1;
	}
	if (modelSize.getElem(2) > currentMaxDimension)
	{
		currentMaxDimension = modelSize.getElem(2);
		biggestDimension = 2;
	}
	if (currentMaxDimension == 0)
		return ;

	switch (biggestDimension)
	{
		case 0:
			resizeX(maxDimension, true);
			break ;
		case 1:
			resizeY(maxDimension, true);
			break ;
		case 2:
			resizeZ(maxDimension, true);
			break ;
		default:
			return ; // Should never happen
	}
}

// Forces the entire model to use the provided Material
// Silently fails if the light has no model
// Throws if the material name is empty or if a material with the same name already existed in the model
void	PointLight::forceMaterial(const std::string &materialName, Material&& material)
{
	//std::cout << "PointLight::forceMaterial: " << materialName << std::endl; // debug
	if (!_model)
		return;
	_model->addMaterial(materialName, std::move(material));
	//std::cout << "Material added to model: " << materialName << std::endl; // debug
	_model->forceMaterial(materialName);
	//std::cout << "Material forced on model: " << materialName << std::endl; // debug
}

/* Attenuation Presets */

void	PointLight::attenuationPreset7() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.7f;
	_attenuation_quadratic = 1.8f;
	_changed = true;
}

void	PointLight::attenuationPreset13() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.35f;
	_attenuation_quadratic = 0.44f;
	_changed = true;
}

void	PointLight::attenuationPreset20() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.22f;
	_attenuation_quadratic = 0.2f;
	_changed = true;
}

void	PointLight::attenuationPreset32() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.14f;
	_attenuation_quadratic = 0.07f;
	_changed = true;
}

void	PointLight::attenuationPreset50() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.09f;
	_attenuation_quadratic = 0.032f;
	_changed = true;
}

void	PointLight::attenuationPreset65() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.07f;
	_attenuation_quadratic = 0.017f;
	_changed = true;
}

void	PointLight::attenuationPreset100() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.045f;
	_attenuation_quadratic = 0.0075f;
	_changed = true;
}

void	PointLight::attenuationPreset160() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.027f;
	_attenuation_quadratic = 0.0028f;
	_changed = true;
}

void	PointLight::attenuationPreset200() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.022f;
	_attenuation_quadratic = 0.0019f;
	_changed = true;
}

void	PointLight::attenuationPreset325() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.014f;
	_attenuation_quadratic = 0.0007f;
	_changed = true;
}

void	PointLight::attenuationPreset600() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.007f;
	_attenuation_quadratic = 0.0002f;
	_changed = true;
}

void	PointLight::attenuationPreset3250() noexcept
{
	_attenuation_constant = 1.0f;
	_attenuation_linear = 0.0014f;
	_attenuation_quadratic = 0.000007f;
	_changed = true;
}
