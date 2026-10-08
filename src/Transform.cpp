/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:24:14 by thepaqui          #+#    #+#             */
/*   Updated: 2024/01/07 01:48:27 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Transform.hpp"

Transform::Transform(const Transform &obj)
{
	this->mat = obj.mat;
}

Transform::Transform(const MATF &matrix)
{
	if (matrix.getRows() != 4 || matrix.getCols() != 4)
		throw std::invalid_argument("Matrix must be 4x4");
	this->mat = matrix;
}

Transform	&Transform::operator=(const Transform &obj)
{
	if (this == &obj)
		return (*this);
	this->mat = obj.mat;
	return (*this);
}

Transform	Transform::operator+(const Transform &obj) const
{
	return Transform(this->mat + obj.mat);
}

Transform	Transform::operator-(const Transform &obj) const
{
	return Transform(this->mat - obj.mat);
}

Transform	Transform::operator*(const float n) const
{
	return Transform(this->mat * n);
}

Transform	Transform::operator*(const Transform &obj) const
{
	return Transform(this->mat * obj.mat);
}

Transform	Transform::operator/(const float n) const
{
	return Transform(this->mat / n);
}

// Model Matrix

void	Transform::scale(const MATF &vec3)
{
	if (MATF::isVec3(vec3) == false)
		throw std::invalid_argument("Bad scaling vector");

	MATF	Scale = MATF::scaling3D(vec3);

	this->mat = Scale * this->mat;
}

// Rotate around X axis
void	Transform::rotateX(const float angleInDegrees)
{
	MATF	Rot = MATF::rotationX3D(angleInDegrees);

	this->mat = Rot * this->mat;
}

// Rotate around Y axis
void	Transform::rotateY(const float angleInDegrees)
{
	MATF	Rot = MATF::rotationY3D(angleInDegrees);

	this->mat = Rot * this->mat;
}

// Rotate around Z axis
void	Transform::rotateZ(const float angleInDegrees)
{
	MATF	Rot = MATF::rotationZ3D(angleInDegrees);

	this->mat = Rot * this->mat;
}

// Rotate around given axis
// Use Transform::vec3() or TVEC3() to get a valid axis easily
// The axis does not need to be normalized beforehand
void	Transform::rotate(const float angleInDegrees, const MATF &axis)
{
	MATF	Rot = MATF::rotation3D(angleInDegrees, axis);

	this->mat = Rot * this->mat;
}

void	Transform::translate(const MATF &vec3)
{
	if (MATF::isVec3(vec3) == false)
		throw std::invalid_argument("Bad translation vector");

	MATF	Trans = MATF::translation3D(vec3);

	this->mat = Trans * this->mat;
}

// View Matrix

void	Transform::lookAt(const MATF &camPos,
	const MATF &target, const MATF &worldUp)
{
	if (MATF::isVec3(camPos) == false
		|| MATF::isVec3(target) == false
		|| MATF::isVec3(worldUp) == false)
		throw std::invalid_argument("Bad camera vectors");

	this->mat = MATF::lookAt(camPos, target, worldUp);
}
