/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 04:39:41 by thepaqui          #+#    #+#             */
/*   Updated: 2026/07/24 18:00:03 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_HPP
# define OBJECT_HPP
# include "Model.hpp"
# include "Texture2D.hpp"
# include "ShaderProgram.hpp"
# include "Transform.hpp"
# include "Material.hpp"

class Scene;

// Default values:
// - scale (1, 1, 1)
// - rotation (0, 0, 0)
// - position (0, 0, 0)
// - visible = true
// - Material = Brass preset
class Object
{
	friend class Scene;

private	:
	Model	*_model = NULL;

	Object	*_parent = NULL;

	std::vector<std::unique_ptr<Object>> _children;

	MATF	_scale = TVEC3(1.0f, 1.0f, 1.0f);
	MATF	_rotation = TVEC3(0.0f, 0.0f, 0.0f);
	MATF	_position = TVEC3(0.0f, 0.0f, 0.0f);
	bool	_visible = true;

	// Extra uniforms
	struct ExtraUniforms {
		bool	b_1 = false;
		bool	b_2 = false;
		bool	b_3 = false;

		float	f_1 = 0.0f;
		float	f_2 = 0.0f;
		float	f_3 = 0.0f;

		MATF	V3_1 = TVEC3(1.0f, 1.0f, 1.0f);
		MATF	V3_2 = TVEC3(1.0f, 1.0f, 1.0f);
		MATF	V3_3 = TVEC3(1.0f, 1.0f, 1.0f);

		Texture*	S2D_1 = nullptr;
		Texture*	S2D_2 = nullptr;
		Texture*	S2D_3 = nullptr;
	} _extraUniforms;

	void	sendModelMatrix(ShaderProgram &sp, const ShaderInfo& shaderInfo, const Transform& modelMat);
	void	sendExtraUniforms(ShaderProgram &sp, const ShaderInfo& shaderInfo);
	void	clearExtraUniformTextures() noexcept;
	void	copyExtraUniformsFrom(const Object &obj);

	void	collectUsedShadersInfo(std::vector<ShaderInfo>& res) const;

protected	:
	const Model	*getModel() const noexcept { return _model; };

public	:
	Object() {};
	Object(Model*const model) { _model = model; };
	Object(const Object &obj);
	~Object();

	Object	&operator=(const Object &obj);

	const MATF&	getPos() const noexcept { return _position; };
	const MATF	&getScale() const noexcept { return _scale; };
	const MATF	&getRot() const noexcept { return _rotation; };
	bool		getVisibility() const noexcept { return _visible; };
	MATF		getSize() const;
	float		getWidth() const noexcept { return getSize().getElem(0); };
	float		getHeight() const noexcept { return getSize().getElem(1); };
	float		getDepth() const noexcept { return getSize().getElem(2); };

	Transform	getLocalTransform() const;
	Transform	getModelTransform() const;

	struct ExtraUniforms& getExtraUniforms() noexcept { return _extraUniforms; };

	std::vector<ShaderInfo>	getUsedShadersInfo() const;

	void	setScale(const MATF &vec3) { _scale = vec3; };
	void	setScale(const float uniformScale)
	{ _scale = TVEC3(uniformScale, uniformScale, uniformScale); };
	void	setRot(const MATF &vec3) { _rotation = vec3; };
	void	setPos(const MATF &vec3) { _position = vec3; };
	void	translate(const MATF &vec3) { _position = _position + vec3; };

	void	resize(const MATF &vec3);
	void	resizeX(const float x, bool keepRatio);
	void	resizeY(const float y, bool keepRatio);
	void	resizeZ(const float z, bool keepRatio);
	void	resizeBiggestDimension(const float maxDimension);

	void	forceMaterial(const std::string &materialName, Material&& material);
	void	forceIlluminationModel(const IlluminationModel newModel);

	Object&			createChild();
	Object&			createChild(Model& model);
	Object*			getParent() noexcept { return _parent; };
	const Object*	getParent() const noexcept { return _parent; };
	const auto&		getChildren() const noexcept { return _children; };
	bool			hasParent() const noexcept { return _parent != NULL; };
	bool			isRoot() const noexcept { return _parent == NULL; };
	bool			isAncestorOf(const Object& obj) const noexcept;

	void	drawOnly(ShaderProgram &shader, const ShaderInfo &shaderInfo, const Transform& parentMat = Transform());

	void	makeVisible() noexcept { _visible = true; };
	void	makeInvisible() noexcept { _visible = false; };
};

#endif
