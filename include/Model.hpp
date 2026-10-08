/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Model.hpp										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: thepaqui <thepaqui@student.42nice.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/01/08 01:59:16 by thepaqui		  #+#	#+#			 */
/*   Updated: 2025/11/07 14:06:42 by thepaqui		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef MODEL_HPP
# define MODEL_HPP
# include "ShaderProgram.hpp"
# include "Texture2D.hpp"
# include "Mesh.hpp"
# include "ObjLoader.hpp"
# include <vector>
# include <unordered_set>
# include <utility>

// Modern 3D model class supporting multiple meshes and materials
class Model
{
private	:
	struct Part {
		Mesh		mesh;
		std::string	name;
		std::string	materialName; // name used in MTL (may be empty)
		Part(Mesh&& m, const std::string &n, const std::string &mat)
			: mesh(std::move(m)), name(n), materialName(mat) {}
	};

	std::string						_directory;
	std::vector<Part>				_parts;
	std::map<std::string, Material>	_materials; // parsed by ObjLoader::loadModel

	MATF	_size = TVEC3(0.0f, 0.0f, 0.0f);

	void	loadModel(const std::string &path, bool centerOnLoad = true);
	void	measureModel(ObjModelData &data);
	void	centerAndMeasureModel(ObjModelData &data);
//	void	processNode(void* node, const void* scene);
//	Mesh	processMesh(void* mesh, const void* scene);
//	std::vector<Texture>	loadMaterialTextures(void* mat, int type, TextureType textureType);

public	:
	Model() {};
	Model(const std::string &path, bool centerOnLoad = true) { loadModel(path, centerOnLoad); };
	void	draw(ShaderProgram &shader);
	void	drawOnly(ShaderProgram &shader, const ShaderInfo& shaderInfo);

	// Note that this returns the MODEL's size, so the scale is 1x1x1 by default.
	// For the associated OBJECT size, use Object::getSize() which applies the scale factor.
	const MATF&	getSize() const;

	std::vector<ShaderInfo>	getUsedShadersInfo() const;

	void	addMaterial(const std::string &name, Material&& material);
	void	forceMaterial(const std::string &materialName);

	void	forceIlluminationModel(const IlluminationModel newModel);

	void	presetPlane(Material&& material, const float textureRepeat = 1.0f);
	void	presetCube(Material&& material);
	void	presetCubeWrap(Material&& material);
};

#endif