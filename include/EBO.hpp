/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EBO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 02:59:20 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:21:32 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EBO_HPP
# define EBO_HPP
# include "glad/glad.hpp"
# include <iostream>
# include <exception>
# include <vector>

class EBO
{
private	:
	bool				_init = false;
	GLuint				_id = 0;
	GLenum				_dataType = GL_UNSIGNED_INT;
	GLsizei				_triangleCount = 0;
	GLsizei				_vertexCount = 0;
	std::vector<GLuint>	_indices;

	void	checkUsage(const GLenum usage) const;

protected	:
	GLenum	getDataType() const noexcept { return _dataType; };
	GLsizei	getTriangleCount() const noexcept { return _triangleCount; };

public	:
	EBO() {};
	EBO(const EBO &obj);
	~EBO() { if (_init) { glDeleteBuffers(1, &_id); }; };

	EBO	&operator=(const EBO &obj);

	bool	getInit() const noexcept { return _init; };
	GLuint	getID() const noexcept { return _id; }
	GLsizei	getVertexCount() const noexcept { return _vertexCount; };

	void		init(const void *data, const size_t dataSize,
		const size_t triangleCount, const GLenum usage,
		const GLenum dataType = GL_UNSIGNED_INT);
	void		init(const std::vector<GLuint> &indices,
		const GLenum usage);

	void	bind() const noexcept;
	void	unbind() const noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };
	void	draw() const noexcept;
	void	reset() noexcept;
};

#endif