/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectionalLight.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:55:38 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 15:05:03 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIONAL_LIGHT_HPP
# define DIRECTIONAL_LIGHT_HPP
# include "ShaderProgram.hpp"
# include "IlluminationModels.hpp"

// Default values:
// - direction (-0.2, -1.0, -0.3)
// - color (1, 1, 1)
//   Display color of the light source
// - colorConvert (0.1, 0.5, 1)
//   see convertColor() for more info on this data
// - ambient intensity (0.1, 0.1, 0.1)
// - diffuse intensity (0.5, 0.5, 0.5)
// - specular intensity (1, 1, 1)
class DirectionalLight
{
private	:
	bool	_active = true;

	MATF	_direction = TVEC3(-0.2f, -1.0f, -0.3f);

	MATF	_color = TVEC3(1.0f, 1.0f, 1.0f);
	MATF	_colorConvert = TVEC3(0.1f, 0.5f, 1.0f);

	MATF	_ambient = TVEC3(0.1f, 0.1f, 0.1f);
	MATF	_diffuse = TVEC3(0.5f, 0.5f, 0.5f);
	MATF	_specular = TVEC3(1.0f, 1.0f, 1.0f);

	// Indicates if light properties have changed since last send to shader
	bool	_changed = true;

	void	setColor(const MATF &vec3) { _color = vec3; };
	void	setColorConvert(const MATF &vec3) { _colorConvert = vec3; };
	void	convertColor();

	void	sendProperties(ShaderProgram &sp, const std::string &prefix);
	void	sendProperties(ShaderProgram &sp, const IlluminationModel illumModel, const std::string &prefix);

protected	:
	const MATF	&getColorConvert() const noexcept { return _colorConvert; };
	const MATF	&getAmbientIntensity() const noexcept { return _ambient; };
	const MATF	&getDiffuseIntensity() const noexcept { return _diffuse; };
	const MATF	&getSpecularIntensity() const noexcept { return _specular; };

public	:
	DirectionalLight() {};
	DirectionalLight(const MATF &direction);
	DirectionalLight(const DirectionalLight &obj);
	~DirectionalLight() {};

	DirectionalLight	&operator=(const DirectionalLight &obj);

	const MATF	&getDirection() const noexcept { return _direction; };
	const MATF	&getColor() const noexcept { return _color; };
	bool		isActive() const noexcept { return _active; };

	void	setActive(const bool active) noexcept { _active = active; };
	void	activate() noexcept { _active = true; };
	void	deactivate() noexcept { _active = false; };
	void	toggleActive() noexcept { _active = !_active; };

	void	setDirection(const MATF &vec3) { _direction = vec3; _changed = true; };
	void	setAmbientIntensity(const MATF &vec3) { _ambient = vec3; _changed = true; };
	void	setDiffuseIntensity(const MATF &vec3) { _diffuse = vec3; _changed = true; };
	void	setSpecularIntensity(const MATF &vec3) { _specular = vec3; _changed = true; };

	void	convertColor(const MATF &newColorVec3);
	void	convertColor(const MATF &newColorVec3, const MATF &newColorConvertVec3);

	void	sendProperties(ShaderProgram &shader)
	{ sendProperties(shader, "directionalLight."); };
	void	sendProperties(ShaderProgram &shader, const IlluminationModel illumModel, unsigned int index)
	{
		const std::string prefix = "directionalLight[" + std::to_string(index) + "].";
		sendProperties(shader, illumModel, prefix);
	};
};

#endif
