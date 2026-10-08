/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 18:53:35 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 19:41:12 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.hpp"

Mesh::Mesh(
	const std::vector<Vertex> &vertices,
	const std::vector<GLuint> &indices
)
{
	_vao = new VAO();
	_vao->bind();

	_vbo = new VBO();
	_vbo->init(vertices, GL_STATIC_DRAW);
	_vbo->bind();

	_ebo = new EBO();
	_ebo->init(indices, GL_STATIC_DRAW);
	_ebo->bind();

	// Each vertex is 8 floats: pos(3) + normal(3) + texCoord(2)
	size_t stride = 8 * sizeof(float);
	
	// Position attribute (location 0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	
	// Normal attribute (location 1)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	
	// TexCoord attribute (location 2)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	_vao->unbind();
}

Mesh::~Mesh()
{
	if (_ebo)
		delete _ebo;
	if (_vbo)
		delete _vbo;
	if (_vao)
		delete _vao;
}

Mesh::Mesh(Mesh &&other) noexcept
	: _vao(other._vao), _vbo(other._vbo), _ebo(other._ebo)
{
	other._vao = nullptr;
	other._vbo = nullptr;
	other._ebo = nullptr;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept
{
	if (this != &other) {
		if (_ebo)
			delete _ebo;
		if (_vbo)
			delete _vbo;
		if (_vao)
			delete _vao;

		_vao = other._vao;
		_vbo = other._vbo;
		_ebo = other._ebo;

		other._vao = nullptr;
		other._vbo = nullptr;
		other._ebo = nullptr;
	}
	return *this;
}

void	Mesh::draw(ShaderProgram &shader, const ShaderInfo& shaderInfo, const Material* material)
{
	if (material) {
		//material->printInfo();
		material->apply(shader, shaderInfo);
	}

	_vao->bind();
	_ebo->draw();
	_vao->unbind();
}
