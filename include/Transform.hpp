/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:13:24 by thepaqui          #+#    #+#             */
/*   Updated: 2026/01/13 19:16:54 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSFORM_HPP
# define TRANSFORM_HPP
# include "matrix/Matrix.hpp"

# define MATF Matrix<float>
# define TVEC3 Transform::vec3
# define TVEC4 Transform::vec4

// This is basically a wrapper around a 4x4 matrix
// With helper functions for common 3D transformations,
// but also view and projection matrices
// Remember to use the SRT order!
// Scale, Rotation, Translation
class Transform
{
private	:
	MATF	mat = MATF(4, 4, Mat_identity);

public	:
	Transform() {};
	Transform(const Transform& obj);
	Transform(const MATF &mat);
	~Transform() {};

	Transform	&operator=(const Transform &obj);
	Transform	operator+(const Transform &obj) const;
	Transform	operator-(const Transform &obj) const;
	Transform	operator*(const float n) const;
	Transform	operator*(const Transform &obj) const;
	Transform	operator/(const float n) const;

	// Resets the matrix back to an identity matrix
	void	reset()
	{ this->mat = MATF(4, 4, Mat_identity); };

	// Resets the matrix back to a null matrix
	void	null()
	{ this->mat = MATF(4, 4, Mat_null); };

	const float	*data() const noexcept
	{ return this->mat.getData(); };

	static float	degToRad(const float degrees) noexcept
	{ return MATF::degToRad(degrees); };

	static float	radToDeg(const float radians) noexcept
	{ return MATF::radToDeg(radians); };

	void	print() const
	{ std::cout << this->mat << std::endl; };

	static MATF	vec3(const float x, const float y, const float z)
	{ return MATF::vec3(x, y, z); };
	static MATF	vec4(const float x, const float y, const float z, const float w)
	{ return MATF::vec4(x, y, z, w); };

	// Model Matrix

	void	scale(const MATF &vec3);

	void	rotateX(const float angleInDegrees);
	void	rotateY(const float angleInDegrees);
	void	rotateZ(const float angleInDegrees);
	void	rotate(const float angleInDegrees, const MATF &axis);

	void	translate(const MATF &vec3);

	// View Matrix

	void	lookAt(const MATF &camPos, const MATF &target, const MATF &worldUp);

	// Projection Matrix

	// Makes an orthographic projection matrix
	// Takes the coordinates of the left, right, top and bottom of the frustum,
	// as well as the Z-coordinates of its near and far faces
	void	orthographic(
		const float left, const float right,
		const float bottom, const float top,
		const float nearZ, const float farZ)
	{ this->mat = MATF::orthographic(left, right, bottom, top, nearZ, farZ); };

	// Makes a perspective projection matrix
	// Takes the vertical FOV (in degrees),
	// the aspect ratio of the viewport (width / height),
	// as well as the Z-coordinates of the near and far faces of the frustum
	// columnMajor is true for column-major matrices, false for row-major matrices
	// rightHanded is true for right-handed coordinate system (+Z-axis points away from the viewer), false for left-handed
	// ndc01Z is true for NDC Z range [0, 1], false for [-1, 1] (false by default for openGL)
	void	perspective(
		const float fovY, const float aspectRatio,
		const float nearZ, const float farZ,
		bool columnMajor = false,
		bool rightHanded = true,
		bool ndc01Z = false)
	{ this->mat = MATF::perspective(fovY, aspectRatio, nearZ, farZ, columnMajor, rightHanded, ndc01Z); };

	// The normal matrix transforms normals from model space to world space
	// So they can be used for lighting calculations even when the model matrix
	// includes non-uniform scaling
	MATF	getNormalMatrix() const
	{
		// Extract upper-left 3x3 from the 4x4 model matrix
		MATF	mat3x3(3, 3, Mat_identity);
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				mat3x3.setElem(i, j, this->mat.getElem(i, j));
			}
		}

		// Compute inverse of 3x3 and transpose
		MATF	mat3x3_inv = MATF::inverse(mat3x3);
		MATF	normalMatrix = MATF::transpose(mat3x3_inv);
		return normalMatrix;
	};
};

#endif
