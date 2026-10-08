/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:41:16 by thepaqui          #+#    #+#             */
/*   Updated: 2026/07/24 18:55:34 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Object.hpp"

// NOTE that this does NOT copy children or the parent.
Object::Object(const Object &obj)
{
	_model = obj._model;
	_scale = obj.getScale();
	_rotation = obj.getRot();
	_position = obj.getPos();
	_visible = obj.getVisibility();
	copyExtraUniformsFrom(obj);
}

// NOTE that this does NOT copy children or the parent.
Object	&Object::operator=(const Object &obj)
{
	if (this != &obj)
	{
		_model = obj._model;
		_scale = obj.getScale();
		_rotation = obj.getRot();
		_position = obj.getPos();
		_visible = obj.getVisibility();
		copyExtraUniformsFrom(obj);
	}
	return (*this);
}

Object::~Object()
{
	clearExtraUniformTextures();
	//std::cout << "Deleting obj with " << _children.size() << " children" << std::endl; // debug
}

void	Object::clearExtraUniformTextures() noexcept
{
	if (_extraUniforms.S2D_1)
	{
		delete _extraUniforms.S2D_1;
		_extraUniforms.S2D_1 = nullptr;
	}
	if (_extraUniforms.S2D_2)
	{
		delete _extraUniforms.S2D_2;
		_extraUniforms.S2D_2 = nullptr;
	}
	if (_extraUniforms.S2D_3)
	{
		delete _extraUniforms.S2D_3;
		_extraUniforms.S2D_3 = nullptr;
	}
}

void	Object::copyExtraUniformsFrom(const Object &obj)
{
	Texture	*newS2D_1 = nullptr;
	Texture	*newS2D_2 = nullptr;
	Texture	*newS2D_3 = nullptr;

	try
	{
		if (obj._extraUniforms.S2D_1)
			newS2D_1 = new Texture(*obj._extraUniforms.S2D_1);
		if (obj._extraUniforms.S2D_2)
			newS2D_2 = new Texture(*obj._extraUniforms.S2D_2);
		if (obj._extraUniforms.S2D_3)
			newS2D_3 = new Texture(*obj._extraUniforms.S2D_3);
	}
	catch (...)
	{
		if (newS2D_1)
			delete newS2D_1;
		if (newS2D_2)
			delete newS2D_2;
		if (newS2D_3)
			delete newS2D_3;
		throw;
	}

	clearExtraUniformTextures();
	_extraUniforms.f_1 = obj._extraUniforms.f_1;
	_extraUniforms.f_2 = obj._extraUniforms.f_2;
	_extraUniforms.f_3 = obj._extraUniforms.f_3;
	_extraUniforms.V3_1 = obj._extraUniforms.V3_1;
	_extraUniforms.V3_2 = obj._extraUniforms.V3_2;
	_extraUniforms.V3_3 = obj._extraUniforms.V3_3;
	_extraUniforms.S2D_1 = newS2D_1;
	_extraUniforms.S2D_2 = newS2D_2;
	_extraUniforms.S2D_3 = newS2D_3;
}

// Returns the size of the object after scaling
// Negative scaling is supported
MATF	Object::getSize() const
{
	if (!_model)
		return TVEC3(0.0f, 0.0f, 0.0f);
	MATF	size = _model->getSize();
	size.setElem(0, fabs(size.getElem(0) * _scale.getElem(0)));
	size.setElem(1, fabs(size.getElem(1) * _scale.getElem(1)));
	size.setElem(2, fabs(size.getElem(2) * _scale.getElem(2)));
	return size;
}

Transform	Object::getLocalTransform() const
{
    Transform t;

    t.rotateX(_rotation.getElem(0));
    t.rotateY(_rotation.getElem(1));
    t.rotateZ(_rotation.getElem(2));

    t.translate(_position);

    return t;
}

Transform	Object::getModelTransform() const
{
    Transform t;

    t.scale(_scale);

    return t;
}

void	Object::collectUsedShadersInfo(std::vector<ShaderInfo>& res) const
{
	if (_model)
	{
		auto shaders = _model->getUsedShadersInfo();

		for (const auto& shader : shaders)
		{
			if (std::find(res.begin(), res.end(), shader) == res.end())
				res.push_back(shader);
		}
	}

	for (const auto& child : _children)
		child->collectUsedShadersInfo(res);
}

// Note that all returned shader info don't contain lights info
// as it's managed at the scene level
// TODO: Might need to query the children also
std::vector<ShaderInfo>	Object::getUsedShadersInfo() const
{
	std::vector<ShaderInfo> res;
	collectUsedShadersInfo(res);
	return res;
}

// Adjusts the object's scale in order make its size match the given values
// If the underlying model has one or more sizes of 0, those dimensions are ignored
// If the model doesn't exist, nothing happens
void	Object::resize(const MATF &vec3)
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
void	Object::resizeX(const float x, bool keepRatio)
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
void	Object::resizeY(const float y, bool keepRatio)
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
void	Object::resizeZ(const float z, bool keepRatio)
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
void	Object::resizeBiggestDimension(const float maxDimension)
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

void	Object::sendModelMatrix(ShaderProgram &sp, const ShaderInfo& shaderInfo, const Transform& modelMat)
{
	if (!_visible || !_model)
	{
		sp.setFloatMat4("model", MATF(4, 4, Mat_null).getData());
	}
	else
	{
		sp.setFloatMat4("model", modelMat.data());

		if (doesIlluminationModelUseNormalMatrix(shaderInfo.getIlluminationModel()))
		{
			MATF	normalMatrix = modelMat.getNormalMatrix();
			sp.setFloatMat3("normalMatrix", normalMatrix.getData());
		}
	}
}

void	Object::sendExtraUniforms(ShaderProgram &sp, const ShaderInfo& shaderInfo)
{
	const IlluminationModel model = shaderInfo.getIlluminationModel();

	if (doesIlluminationModelUseB_1(model))
		sp.setBool("b_1", _extraUniforms.b_1);
	if (doesIlluminationModelUseB_2(model))
		sp.setBool("b_2", _extraUniforms.b_2);
	if (doesIlluminationModelUseB_3(model))
		sp.setBool("b_3", _extraUniforms.b_3);

	if (doesIlluminationModelUseF_1(model))
		sp.setFloat("f_1", _extraUniforms.f_1);
	if (doesIlluminationModelUseF_2(model))
		sp.setFloat("f_2", _extraUniforms.f_2);
	if (doesIlluminationModelUseF_3(model))
		sp.setFloat("f_3", _extraUniforms.f_3);

	if (doesIlluminationModelUseV3_1(model))
		sp.setFloatVec3("v3_1", _extraUniforms.V3_1);
	if (doesIlluminationModelUseV3_2(model))
		sp.setFloatVec3("v3_2", _extraUniforms.V3_2);
	if (doesIlluminationModelUseV3_3(model))
		sp.setFloatVec3("v3_3", _extraUniforms.V3_3);

	if (doesIlluminationModelUseS2D_1(model))
	{
		if (_extraUniforms.S2D_1)
		{
			const GLuint unit = TEXTURE_UNIT_CUSTOM_START;
			_extraUniforms.S2D_1->activate(unit);
			_extraUniforms.S2D_1->use();
			sp.setInt("s2d_1", unit);
		}
	}
	if (doesIlluminationModelUseS2D_2(model))
	{
		if (_extraUniforms.S2D_2)
		{
			const GLuint unit = TEXTURE_UNIT_CUSTOM_START + 1;
			_extraUniforms.S2D_2->activate(unit);
			_extraUniforms.S2D_2->use();
			sp.setInt("s2d_2", unit);
		}
	}
	if (doesIlluminationModelUseS2D_3(model))
	{
		if (_extraUniforms.S2D_3)
		{
			const GLuint unit = TEXTURE_UNIT_CUSTOM_START + 2;
			_extraUniforms.S2D_3->activate(unit);
			_extraUniforms.S2D_3->use();
			sp.setInt("s2d_3", unit);
		}
	}
}

Object&	Object::createChild()
{
	std::unique_ptr<Object>	newObject = std::make_unique<Object>();
	newObject->_parent = this;
	
	Object&	objRef = *newObject;
	_children.push_back(std::move(newObject));
	return objRef;
}

Object&	Object::createChild(Model& model)
{
	std::unique_ptr<Object>	newObject = std::make_unique<Object>(&model);
	newObject->_parent = this;
	
	Object&	objRef = *newObject;
	_children.push_back(std::move(newObject));
	return objRef;
}

bool	Object::isAncestorOf(const Object& obj) const noexcept
{
	const Object*	current = obj._parent;

	while (current)
	{
		if (current == this)
			return true;
		current = current->_parent;
	}
	return false;
}

// This only takes into account the illumination model
// And the maps used
void Object::drawOnly(ShaderProgram &shader, const ShaderInfo &shaderInfo, const Transform& parentMat)
{
	Transform	world = parentMat * getLocalTransform(); // Only position and rotation are inherited
	Transform	model = world * getModelTransform(); // Scale is NOT inherited

	if (_visible && _model)
	{
		sendModelMatrix(shader, shaderInfo, model);
		sendExtraUniforms(shader, shaderInfo);
		_model->drawOnly(shader, shaderInfo);
	}

	for (auto& child : _children)
	{
		child->drawOnly(shader, shaderInfo, world);
	}
}

// Forces the entire model to use the provided Material
// Silently fails if the object has no model
// Throws if the material name is empty or if a material with the same name already existed in the model
void	Object::forceMaterial(const std::string &materialName, Material&& material)
{
	if (!_model)
		return;
	_model->addMaterial(materialName, std::move(material));
	_model->forceMaterial(materialName);
}

// Forces the entire model to use the provided IlluminationModel
// Silently fails if the object has no model
void	Object::forceIlluminationModel(const IlluminationModel newModel)
{
	if (!_model)
		return;
	_model->forceIlluminationModel(newModel);
}