/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:10:14 by thepaqui          #+#    #+#             */
/*   Updated: 2026/07/24 17:35:12 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_HPP
# define SCENE_HPP
# include <memory>
# include <string>
# include <vector>
# include <map>
# include <variant>
# include "ShaderManager.hpp"
# include "Model.hpp"
# include "Object.hpp"
# include "PointLight.hpp"
# include "DirectionalLight.hpp"
# include "Spotlight.hpp"
# include "Camera.hpp"
# include "Transform.hpp"
# include "UserInput.hpp"

class Scene
{
private	:
	/* --- Scene Data --- */

	ShaderManager					_shaderManager;
	std::vector<IlluminationModel>	_defaultShadersUsed;
	std::vector<size_t>				_customShadersUsed;

	// Should we allow multiple cameras with unique IDs?
	// (probably, with an option to switch between them)
	Camera		_camera;
	// Would multiple projections make sense?
	Transform	_projection;
	t_UserInput	_userInput;

	// Render states
	bool	_depthTest = true;
	bool	_wireframe = false;
	bool	_faceCulling = true;
	bool	_cullBackFaces = true;

	MATF	_clearColor = TVEC4(0.0f, 0.0f, 0.0f, 1.0f); // Black by default

	// Allows for reusing materials across multiple models
	// TODO
	//std::map<std::string, std::unique_ptr<Material>>			_materials;

	// The model parsing from file should store materials in the SceneManager
	// so that multiple models can share the same material instances
	std::map<std::string, std::unique_ptr<Model>>	_models;

	std::vector<std::unique_ptr<Object>>			_objects;
	// This should come with a good system to have multiple
	// lights of each type in the actual shaders
	std::vector<std::unique_ptr<PointLight>>		_pointLights;
	std::vector<std::unique_ptr<DirectionalLight>>	_directionalLights;
	std::vector<std::unique_ptr<Spotlight>>			_spotlights;

	/* --- Private Methods --- */

	Model&	addModel(const std::string& path);
	Model&	addModelForce(const std::string& path);

	// Send scene data to the given shader program
	void	sendToShader(IlluminationModel illumModel, ShaderProgram& shader);
	void	applyRenderState() const;

public	:
	Scene();
	~Scene();
	// Delete copy and move constructors and assignment operators
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene&	operator=(const Scene&) = delete;
	Scene&	operator=(Scene&&) = delete;

	ShaderManager	&getShaderManager() noexcept { return _shaderManager; };

	Camera		&getCamera() noexcept { return _camera; };
	Transform	&getProjection() noexcept { return _projection; };
	bool		isDepthTestEnabled() const noexcept { return _depthTest; };
	bool		isWireframeEnabled() const noexcept { return _wireframe; };
	bool		isFaceCullingEnabled() const noexcept { return _faceCulling; };
	bool		isCullingBackFaces() const noexcept { return _cullBackFaces; };
	bool		isCullingFrontFaces() const noexcept { return !_cullBackFaces; };
	const MATF	&getClearColor() noexcept { return _clearColor; };
	float		getClearColorRed() noexcept { return _clearColor.getElem(0); };
	float		getClearColorGreen() noexcept { return _clearColor.getElem(1); };
	float		getClearColorBlue() noexcept { return _clearColor.getElem(2); };
	float		getClearColorAlpha() noexcept { return _clearColor.getElem(3); };
	t_UserInput	&getUserInput() noexcept { return _userInput; };

	Model&	getModel(const std::string& modelPath);

	void	setClearColor(const MATF &color);
	void	setClearColor(const float r, const float g, const float b, const float a = 1.0f);
	void	setClearColorRed(const float r);
	void	setClearColorGreen(const float g);
	void	setClearColorBlue(const float b);
	void	setClearColorAlpha(const float a);
	void	setDepthTest(const bool enabled) noexcept { _depthTest = enabled; };
	void	toggleDepthTest() noexcept { _depthTest = !_depthTest; };
	void	setWireframe(const bool enabled) noexcept { _wireframe = enabled; };
	void	toggleWireframe() noexcept { _wireframe = !_wireframe; };
	void	setFaceCulling(const bool enabled) noexcept { _faceCulling = enabled; };
	void	toggleFaceCulling() noexcept { _faceCulling = !_faceCulling; };
	void	cullBackFaces() noexcept { _cullBackFaces = true; };
	void	cullFrontFaces() noexcept { _cullBackFaces = false; };
	void	swapCulledFaces() noexcept { _cullBackFaces = !_cullBackFaces; };

	const std::string	addPlaneModel(Material&& material, const float textureRepeat = 1.0f);
	const std::string	addCubeModel(Material&& material, const bool wrap = false);

	Object&	addObject(const std::string& modelPath, bool forceful = false);
	Object&	addObject(Model& model);
	Object&	addObject();

	void	makeChildOf(Object& parent, Object& child);
	void	takeChild(Object& parent, Object& child);

	PointLight&			addPointLight(const std::string& modelPath, bool forceful = false);
	PointLight&			addPointLight(Model& model);
	PointLight&			addPointLight();
	PointLight&			addPointLight(const PointLight &obj);

	DirectionalLight&	addDirectionalLight();
	DirectionalLight&	addDirectionalLight(const MATF &direction);
	DirectionalLight&	addDirectionalLight(const DirectionalLight &obj);

	Spotlight&			addSpotlight();
	Spotlight&			addSpotlight(const Spotlight &obj);

	void	render();
};

#endif