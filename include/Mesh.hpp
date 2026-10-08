/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 18:33:59 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/03 14:48:09 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP
# include "Texture2D.hpp"
# include "VAO.hpp"
# include "VBO.hpp"
# include "EBO.hpp"
# include "ShaderProgram.hpp"
# include "Material.hpp"
# include <vector>
# include <map>

static std::map<TextureType, std::string> textureTypeToUniformName = {
	{TEXTURE_TYPE_AMBIENT, "texture_ambient"},
	{TEXTURE_TYPE_DIFFUSE, "texture_diffuse"},
	{TEXTURE_TYPE_SPECULAR, "texture_specular"},
	{TEXTURE_TYPE_EMISSION, "texture_emission"},
	{TEXTURE_TYPE_NORMAL, "texture_normal"},
	{TEXTURE_TYPE_HEIGHT, "texture_height"},
	{TEXTURE_TYPE_AMBIENT_OCCLUSION, "texture_ambient_occlusion"},
	{TEXTURE_TYPE_ROUGHNESS, "texture_roughness"},
	{TEXTURE_TYPE_METALLIC, "texture_metallic"}
};

class Mesh
{
private	:
	VAO*	_vao = nullptr;
	VBO*	_vbo = nullptr;
	EBO*	_ebo = nullptr;

public	:
	Mesh(
		const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices
	);
	~Mesh();

	// Non-copyable (owns GL resources)
	Mesh(const Mesh &other) = delete;
	Mesh &operator=(const Mesh &other) = delete;

	// Movable: transfer resource ownership
	Mesh(Mesh &&other) noexcept;
	Mesh &operator=(Mesh &&other) noexcept;

	// Draw with optional material. If provided, the material's texture units
	// and shininess will be applied before drawing.
	void	draw(ShaderProgram &shader, const ShaderInfo& shaderInfo, const Material* material = nullptr);
};

# endif