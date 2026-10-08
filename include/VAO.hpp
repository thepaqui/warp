/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:19:23 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:14:27 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAO_HPP
# define VAO_HPP
# include "glad/glad.hpp"

class VAO
{
private	:
	GLuint	_id = 0;

public	:
	VAO();
	~VAO();

	void	bind() { glBindVertexArray(_id); };
	void	unbind() { glBindVertexArray(0); };
	GLuint	getID() const noexcept { return _id; };
};

#endif