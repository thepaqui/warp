/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:46:04 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 17:39:32 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VBO.hpp"

VBO::VBO(const VBO &obj)
{
	_init = obj.getInit();
	_id = obj.getID();
	_startIndex = obj.getStartIndex();
	_vertexCount = obj.getVertexCount();
}

VBO	&VBO::operator=(const VBO &obj)
{
	if (this != &obj)
	{
		_init = obj.getInit();
		_id = obj.getID();
		_startIndex = obj.getStartIndex();
		_vertexCount = obj.getVertexCount();
	}
	return (*this);
}

// Exports data to the GPU (GL_ARRAY_BUFFER)
// Unbinds VBO
// You should use the other init() method with a vector of Vertex structs
// TODO: Remove this overload in the future
void	VBO::init(const void *data, const size_t dataSize,
	const size_t vertexCount, const GLenum usage, const GLint startIndex)
{
	if (_init)
		return ;
	if (!data)
	{
		std::cerr << "[VBO] DATA PTR IS NULL" << std::endl;
		throw std::invalid_argument("VBO: data ptr is null");
	}
	checkUsage(usage);

	glGenBuffers(1, &_id);
	_vertexCount = vertexCount;
	_startIndex = startIndex;
	_init = true;
	bind();

	glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);

	unbind();
}

// Exports data to the GPU (GL_ARRAY_BUFFER)
// Unbinds VBO
void	VBO::init(
	const std::vector<Vertex> &vertices,
	const GLenum usage,
	const GLint startIndex
)
{
	if (_init)
		return ;
	_vertices = vertices;
	if (_vertices.empty())
	{
		std::cerr << "[VBO] VERTEX VECTOR IS EMPTY" << std::endl;
		throw std::invalid_argument("VBO: vertex vector is empty");
	}
	checkUsage(usage);

	glGenBuffers(1, &_id);
	_vertexCount = static_cast<GLsizei>(_vertices.size());
	_startIndex = startIndex;
	_init = true;
	bind();

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
		&_vertices[0], usage);

	unbind();
}

void	VBO::checkUsage(const GLenum usage) const
{
	const GLenum	valid[9] = {
		GL_STREAM_COPY,
		GL_STREAM_DRAW,
		GL_STREAM_READ,
		GL_STATIC_COPY,
		GL_STATIC_DRAW,
		GL_STATIC_READ,
		GL_DYNAMIC_COPY,
		GL_DYNAMIC_DRAW,
		GL_DYNAMIC_READ
	};

	for (int i = 0; i < 9; i++)
		if (usage == valid[i])
			return ;

	std::cerr << "[VBO] BAD USAGE VALUE" << std::endl;
	throw std::invalid_argument("VBO: Bad usage value");
}

void	VBO::bind() const noexcept
{
	if (!_init)
		return ;
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void	VBO::draw() const noexcept
{
	if (!_init)
		return ;
	glDrawArrays(GL_TRIANGLES, _startIndex, _vertexCount);
}

void	VBO::reset() noexcept
{
	if (!_init)
		return ;
	glDeleteBuffers(1, &_id);
	_init = false;
	_id = 0;
	_startIndex = 0;
	_vertexCount = 0;
	_vertices.clear();
}
