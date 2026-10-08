/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:05:42 by thepaqui          #+#    #+#             */
/*   Updated: 2026/10/08 18:21:24 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"

Scene::Scene()
: _shaderManager("./warp/shaders/templates", "./warp/shaders")
{}

Scene::~Scene()
{
	// Unique pointers clean up automatically
}

Model&	Scene::getModel(const std::string& modelPath)
{
	return addModel(modelPath);
}

// This can throw if the passed color is not a vec4
// Using the overload with separate r, g, b and a parameters is safer
void	Scene::setClearColor(const MATF &color)
{
	if (MATF::isVec4(color) == false)
		throw std::runtime_error("[SCENE] INVALID CLEAR COLOR: MUST BE A VEC4");
	setClearColorRed(color.getElem(0));
	setClearColorGreen(color.getElem(1));
	setClearColorBlue(color.getElem(2));
	setClearColorAlpha(color.getElem(3));
}

void	Scene::setClearColor(const float r, const float g, const float b, const float a)
{
	setClearColorRed(r);
	setClearColorGreen(g);
	setClearColorBlue(b);
	setClearColorAlpha(a);
}

void	Scene::setClearColorRed(const float r)
{
	_clearColor.setElem(0, std::clamp(r, 0.0f, 1.0f));
}

void	Scene::setClearColorGreen(const float g)
{
	_clearColor.setElem(1, std::clamp(g, 0.0f, 1.0f));
}

void	Scene::setClearColorBlue(const float b)
{
	_clearColor.setElem(2, std::clamp(b, 0.0f, 1.0f));
}

void	Scene::setClearColorAlpha(const float a)
{
	_clearColor.setElem(3, std::clamp(a, 0.0f, 1.0f));
}

// If the model was already loaded, returns the existing one
// All models are forcefully centered
Model&	Scene::addModel(const std::string& path)
{
	auto	it = _models.find(path);
	if (it != _models.end())
		return *(it->second);

	std::unique_ptr<Model>	newModel = std::make_unique<Model>(path);
	Model&					modelRef = *newModel;

	_models.emplace(path, std::move(newModel));
	return modelRef;
}

// Creates a new model even if the model was already loaded
// All models are forcefully centered
Model&	Scene::addModelForce(const std::string& path)
{
	unsigned int nb = 0;
	std::string	suffix = "";
	std::string	key = path + suffix;

	auto	it = _models.find(key);
	while (it != _models.end())
	{
		++nb;
		suffix = "_" + std::to_string(nb);
		key = path + suffix;
		it = _models.find(key);
	}

	std::unique_ptr<Model>	newModel = std::make_unique<Model>(path);
	Model&					modelRef = *newModel;

	_models.emplace(key, std::move(newModel));
	return modelRef;
}

const std::string	Scene::addPlaneModel(Material&& material, const float textureRepeat)
{
	unsigned int nb = 0;
	std::string keyPrefix = "!presetPlane_" + std::to_string(textureRepeat) + "_";
	std::string key = keyPrefix + std::to_string(nb);
	while (_models.find(key) != _models.end())
	{
		++nb;
		key = keyPrefix + std::to_string(nb);
	}

	std::unique_ptr<Model>	newModel = std::make_unique<Model>();
	Model&					modelRef = *newModel;

	modelRef.presetPlane(std::move(material), textureRepeat);
	_models.emplace(key, std::move(newModel));
	return key;
}

const std::string	Scene::addCubeModel(Material&& material, const bool wrap)
{
	unsigned int nb = 0;
	std::string keyPrefix = wrap ? "!presetCubeWrap_" : "!presetCube_";
	std::string key = keyPrefix + std::to_string(nb);
	while (_models.find(key) != _models.end())
	{
		++nb;
		key = keyPrefix + std::to_string(nb);
	}

	std::unique_ptr<Model>	newModel = std::make_unique<Model>();
	Model&					modelRef = *newModel;

	if (wrap)
		modelRef.presetCubeWrap(std::move(material));
	else
		modelRef.presetCube(std::move(material));
	_models.emplace(key, std::move(newModel));
	return key;
}

// This returns a reference to the created object
// If the path is invalid, an exception will be thrown by addModel
// forceful = true will create a new model even if the model was already loaded
Object&	Scene::addObject(const std::string& modelPath, bool forceful)
{
	Model&	model = forceful ? addModelForce(modelPath) : addModel(modelPath);

	std::unique_ptr<Object>	newObject = std::make_unique<Object>(&model);
	Object&					objRef = *newObject;

	_objects.emplace_back(std::move(newObject));
	return objRef;
}

// This returns a reference to the created object
Object&	Scene::addObject(Model& model)
{
	std::unique_ptr<Object>	newObject = std::make_unique<Object>(&model);
	Object&					objRef = *newObject;

	_objects.emplace_back(std::move(newObject));
	return objRef;
}

// This returns a reference to the created object
Object&	Scene::addObject()
{
	std::unique_ptr<Object>	newObject = std::make_unique<Object>();
	Object&					objRef = *newObject;

	_objects.emplace_back(std::move(newObject));
	return objRef;
}

void	Scene::makeChildOf(Object& parent, Object& child)
{
	if (&parent == &child || child.hasParent() || child.isAncestorOf(parent))
		return ;
	
	auto it = std::find_if(
		_objects.begin(),
		_objects.end(),
		[&](const auto& ptr) {
			return ptr.get() == &child;
		}
	);

	if (it != _objects.end())
	{
		auto object = std::move(*it);
		object->_parent = &parent;
		parent._children.push_back(std::move(object));
		_objects.erase(it);
	}
}

void	Scene::takeChild(Object& parent, Object& child)
{
	if (&parent == &child || !child.hasParent() || child.getParent() != &parent)
		return ;

	auto& pchildren = parent._children;

	auto it = std::find_if(
		pchildren.begin(),
		pchildren.end(),
		[&](const auto& ptr) {
			return ptr.get() == &child;
		}
	);

	if (it != pchildren.end())
	{
		auto object = std::move(*it);
		object->_parent = NULL;
		_objects.push_back(std::move(object));
		pchildren.erase(it);
	}
}

PointLight&	Scene::addPointLight(const std::string& modelPath, bool forceful)
{
	Model&	model = forceful ? addModelForce(modelPath) : addModel(modelPath);

	std::unique_ptr<PointLight>	newLight = std::make_unique<PointLight>(&model);
	PointLight&					lightRef = *newLight;

	_pointLights.emplace_back(std::move(newLight));
	return lightRef;
}

PointLight&	Scene::addPointLight(Model& model)
{
	std::unique_ptr<PointLight>	newLight = std::make_unique<PointLight>(&model);
	PointLight&					lightRef = *newLight;

	_pointLights.emplace_back(std::move(newLight));
	return lightRef;
}

PointLight&	Scene::addPointLight()
{
	std::unique_ptr<PointLight>	newLight = std::make_unique<PointLight>();
	PointLight&					lightRef = *newLight;

	_pointLights.emplace_back(std::move(newLight));
	return lightRef;
}

PointLight&	Scene::addPointLight(const PointLight &obj)
{
	std::unique_ptr<PointLight>	newLight = std::make_unique<PointLight>(obj);
	PointLight&					lightRef = *newLight;

	_pointLights.emplace_back(std::move(newLight));
	return lightRef;
}

DirectionalLight&	Scene::addDirectionalLight()
{
	std::unique_ptr<DirectionalLight>	newLight = std::make_unique<DirectionalLight>();
	DirectionalLight&					lightRef = *newLight;

	_directionalLights.emplace_back(std::move(newLight));
	return lightRef;
}

DirectionalLight&	Scene::addDirectionalLight(const MATF &direction)
{
	std::unique_ptr<DirectionalLight>	newLight = std::make_unique<DirectionalLight>(direction);
	DirectionalLight&					lightRef = *newLight;

	_directionalLights.emplace_back(std::move(newLight));
	return lightRef;
}

DirectionalLight&	Scene::addDirectionalLight(const DirectionalLight &obj)
{
	std::unique_ptr<DirectionalLight>	newLight = std::make_unique<DirectionalLight>(obj);
	DirectionalLight&					lightRef = *newLight;

	_directionalLights.emplace_back(std::move(newLight));
	return lightRef;
}

Spotlight&	Scene::addSpotlight()
{
	std::unique_ptr<Spotlight>	newLight = std::make_unique<Spotlight>();
	Spotlight&					lightRef = *newLight;

	_spotlights.emplace_back(std::move(newLight));
	return lightRef;
}

Spotlight&	Scene::addSpotlight(const Spotlight &obj)
{
	std::unique_ptr<Spotlight>	newLight = std::make_unique<Spotlight>(obj);
	Spotlight&					lightRef = *newLight;

	_spotlights.emplace_back(std::move(newLight));
	return lightRef;
}

void Scene::sendToShader(IlluminationModel illumModel, ShaderProgram& shader)
{
	// Camera and projection
	shader.use();
	if (doesIlluminationModelUseCamPos(illumModel))
		_camera.sendPos(shader, "camPos");
	shader.setFloatMat4("view", _camera.data());
	shader.setFloatMat4("projection", _projection.data());

	// Send lights properties
	for (unsigned int i = 0; i < _pointLights.size(); ++i)
		_pointLights[i]->sendProperties(shader, illumModel, i);
	for (unsigned int i = 0; i < _directionalLights.size(); ++i)
		_directionalLights[i]->sendProperties(shader, illumModel, i);
	for (unsigned int i = 0; i < _spotlights.size(); ++i)
		_spotlights[i]->sendProperties(shader, illumModel, i);
}

void	Scene::applyRenderState() const
{
	if (_depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if (_faceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(_cullBackFaces ? GL_BACK : GL_FRONT);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
}

// TODO : Caching shaders used and number of lights in scene to avoid doing all of that every frame
void	Scene::render()
{
	applyRenderState();

	// Clear color on the screen
	glClearColor(_clearColor.getElem(0), _clearColor.getElem(1), _clearColor.getElem(2), _clearColor.getElem(3));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	using Drawable = std::variant<Object*, PointLight*>;

	// Map shaders to objects using them
	std::map<ShaderInfo, std::vector<Drawable>>			shaderInfoToObjectsMap;

	unsigned int	dirLightCount = static_cast<unsigned int>(_directionalLights.size());
	unsigned int	pointLightCount = static_cast<unsigned int>(_pointLights.size());
	unsigned int	spotlightCount = static_cast<unsigned int>(_spotlights.size());

	// Add objects to the maps
	for (const auto& object : _objects)
	{
		std::vector<ShaderInfo> usedShadersInfo = object->getUsedShadersInfo();
		for (ShaderInfo& shaderInfo : usedShadersInfo)
		{
			shaderInfo.setLightCounts(dirLightCount, pointLightCount, spotlightCount);
			shaderInfoToObjectsMap[shaderInfo].push_back(object.get());
		}
	}

	// Add point lights to the maps
	for (const auto& light : _pointLights)
	{
		std::vector<ShaderInfo> usedShadersInfo = light->getUsedShadersInfo();
		for (ShaderInfo& shaderInfo : usedShadersInfo)
		{
			shaderInfo.setLightCounts(dirLightCount, pointLightCount, spotlightCount);
			shaderInfoToObjectsMap[shaderInfo].push_back(light.get());
		}
	}

	// Draw objects and point lights
	for (auto& shaderInfoToObjects : shaderInfoToObjectsMap)
	{
		ShaderInfo shaderInfo = shaderInfoToObjects.first;
		if (!_shaderManager.hasShaderProgram(shaderInfo))
			_shaderManager.addShaderProgram(shaderInfo);
		ShaderProgram& shader = _shaderManager.getShaderProgram(shaderInfo);

		IlluminationModel	illumModel = shaderInfo.getIlluminationModel();

		// Camera, projection and lights
		sendToShader(illumModel, shader);

		std::vector<Drawable>& objects = shaderInfoToObjects.second;
		// Draw objects
		for (Drawable& object : objects)
		{
			std::visit([&shader, shaderInfo](auto obj)
			{ obj->drawOnly(shader, shaderInfo); }, object);
		}
	}
}