/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:45:25 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:14:41 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VBO_HPP
# define VBO_HPP
# include "glad/glad.hpp"
# include "Transform.hpp"
# include <exception>

// struct for OpenGL vertex data
// Memory layout: position(3 floats) + normal(3 floats) + texCoords(2 floats)
struct Vertex
{
	float	position[3];
	float	normal[3];
	float	texCoords[2];
};

// DO NOT USE NAKED VBOs
class VBO
{
private	:
	bool				_init = false;
	GLuint				_id = 0;
	GLint				_startIndex = 0;
	GLsizei				_vertexCount = 0;
	std::vector<Vertex>	_vertices;

	void	checkUsage(const GLenum usage) const;

protected	:
	GLint	getStartIndex() const noexcept { return _startIndex; };

public	:
	VBO() {};
	VBO(const VBO &obj);
	~VBO() { reset(); };

	VBO	&operator=(const VBO &obj);

	bool	getInit() const noexcept { return _init; };
	GLuint	getID() const noexcept { return _id; }
	GLsizei	getVertexCount() const noexcept { return _vertexCount; };

	void	init(const void *data, const size_t dataSize,
		const size_t vertexCount, const GLenum usage,
		const GLint startIndex = 0);
	void	init(const std::vector<Vertex> &vertices,
		const GLenum usage,
		const GLint startIndex = 0);

	void	bind() const noexcept;
	void	unbind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); };
	void	draw() const noexcept;
	void	reset() noexcept;
};

#endif