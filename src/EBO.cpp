/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 02:59:51 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 17:32:58 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EBO.hpp"

EBO::EBO(const EBO &obj)
{
	_init = obj.getInit();
	_id = obj.getID();
	_dataType = obj.getDataType();
	_triangleCount = obj.getTriangleCount();
	_vertexCount = obj.getVertexCount();
}

EBO	&EBO::operator=(const EBO &obj)
{
	if (this != &obj)
	{
		_init = obj.getInit();
		_id = obj.getID();
		_dataType = obj.getDataType();
		_triangleCount = obj.getTriangleCount();
		_vertexCount = obj.getVertexCount();
	}
	return (*this);
}

// Exports data to the GPU (GL_ELEMENT_ARRAY_BUFFER)
// Unbinds EBO
// You should use the other init() method with a vector of indices
// TODO: Remove this overload in the future
void	EBO::init(const void *data, const size_t dataSize,
	const size_t triangleCount, const GLenum usage, const GLenum dataType)
{
	if (!data)
	{
		std::cerr << "[EBO] DATA PTR IS NULL" << std::endl;
		throw std::invalid_argument("EBO: data ptr is null");
	}
	if (dataType != GL_UNSIGNED_BYTE
		&& dataType != GL_UNSIGNED_SHORT
		&& dataType != GL_UNSIGNED_INT)
	{
		std::cerr << "[EBO] BAD DATA TYPE" << std::endl;
		throw std::invalid_argument("EBO: bad data type");
	}
	checkUsage(usage);

	glGenBuffers(1, &_id);
	_triangleCount = triangleCount;
	_vertexCount = triangleCount * 3;
	_dataType = dataType;
	_init = true;
	bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);

	unbind();
}

// Exports data to the GPU (GL_ELEMENT_ARRAY_BUFFER)
// Unbinds EBO
void	EBO::init(const std::vector<GLuint> &indices, const GLenum usage)
{
	_indices = indices;
	if (_indices.empty())
	{
		std::cerr << "[EBO] INDEX VECTOR IS EMPTY" << std::endl;
		throw std::invalid_argument("EBO: index vector is empty");
	}
	checkUsage(usage);

	glGenBuffers(1, &_id);
	_triangleCount = static_cast<GLsizei>(_indices.size() / 3);
	_vertexCount = static_cast<GLsizei>(_indices.size());
	_dataType = GL_UNSIGNED_INT;
	_init = true;
	bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint),
		&_indices[0], usage);

	unbind();
}



void	EBO::checkUsage(const GLenum usage) const
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

	std::cerr << "[EBO] BAD USAGE VALUE" << std::endl;
	throw std::invalid_argument("EBO: Bad usage value");
}

void	EBO::bind() const noexcept
{
	if (!_init)
		return ;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void	EBO::draw() const noexcept
{
	if (!_init)
		return ;
	glDrawElements(GL_TRIANGLES, _vertexCount, _dataType, NULL);
}

void	EBO::reset() noexcept
{
	if (!_init)
		return ;
	glDeleteBuffers(1, &_id);
	_init = false;
	_id = 0;
	_dataType = GL_UNSIGNED_INT;
	_triangleCount = 0;
	_vertexCount = 0;
	_indices.clear();
}
