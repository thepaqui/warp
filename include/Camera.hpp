/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 01:00:29 by thepaqui          #+#    #+#             */
/*   Updated: 2024/03/12 14:54:08 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP
# include "Transform.hpp"
# include "Object.hpp"
# include "UserInput.hpp"
# include <algorithm>

// Default values:
// - pos is (0, 0, 0)
// - dir is (0, 0, -1)
// - up is (0, 1, 0)
// - speed is 6.0
// - yaw is -90
// - pitch is 0
// - roll is 0
class Camera
{
private	:
	Transform	_view;
	MATF		_pos = TVEC3(0.0f, 0.0f, 0.0f);
	MATF		_dir = TVEC3(0.0f, 0.0f, -1.0f);
	MATF		_up = TVEC3(0.0f, 1.0f, 0.0f);
	float		_speed = 6.0f;

	float	_realSpeed = 0.3f;
	float	_deltaTime = 0.0f;
	float	_lastFrame = 0.0f;
	void	calcSpeed() noexcept { _realSpeed = _speed * _deltaTime; };

	float	_yaw = -1.5708f; // -90 degrees
	float	_pitch = 0.0f;
	float	_roll = 0.0f; // unused for now, no influence on direction
	void	calcDir();
	void	calcAngle() noexcept;

	float	_fov = 45.0f;

public	:
	Camera() { _view.lookAt(_pos, _dir, _up); };
	~Camera() {};

	/* Getters */

	const Transform	&getViewMatrix() const noexcept { return _view; };
	const float		*data() const noexcept { return _view.data(); };

	const MATF	&getPos() const noexcept { return _pos; };
	float		getPosX() const noexcept { return _pos.getElem(0); };
	float		getPosY() const noexcept { return _pos.getElem(1); };
	float		getPosZ() const noexcept { return _pos.getElem(2); };

	const MATF	&getDir() const noexcept { return _dir; };
	const MATF	&getUp() const noexcept { return _up; };
	float		getSpeed() const noexcept { return _speed; };
	float		getYaw() const noexcept { return _yaw; };
	float		getPitch() const noexcept { return _pitch; };
	float		getRoll() const noexcept { return _roll; };
	float		getFOV() const noexcept { return _fov; };

	/* Setters */

	void	setPos(const MATF &newPos) { _pos = newPos; };
	void	setPosX(const float posX) { _pos.setElem(0, posX); };
	void	setPosY(const float posY) { _pos.setElem(1, posY); };
	void	setPosZ(const float posZ) { _pos.setElem(2, posZ); };

	void	setTarget(const MATF &target);
	void	setTarget(const Object &obj) { setTarget(obj.getPos()); };

	void	setUp(const MATF &newUp) { _up = newUp; };
	void	flipView();

	void	setAngle(const float yaw, const float pitch = 0.0f);
	// This takes degrees, not radians!
	void	setRoll(const float roll) noexcept { _roll = MATF::degToRad(roll); }
	void	setFOV(const float fov) noexcept { _fov = fov; };

	void	setSpeed(const float newSpeed) noexcept { _speed = newSpeed; };
	void	setDeltaTime() noexcept;

	void	sendPos(ShaderProgram &sp, const std::string &uniformName) const
	{ sp.setFloatVec3(uniformName, _pos); };

	/* Movement */

	void	goForward() { _pos = _pos + (_dir * _realSpeed); };
	void	goBackward() { _pos = _pos - (_dir * _realSpeed); };
	void	goLeft()
	{ _pos = _pos - (MATF::normalize(MATF::cross(_dir, _up)) * _realSpeed); };
	void	goRight()
	{ _pos = _pos + (MATF::normalize(MATF::cross(_dir, _up)) * _realSpeed); };
	void	goUp() { setPosY(getPosY() + _realSpeed); };
	void	goDown() { setPosY(getPosY() - _realSpeed); };

	/* Modifying the view matrix */

	void	lookAt();
	// Turns the camera towards the given position
	void	lookAt(const MATF &target) { setTarget(target); lookAt(); };
	// Turns the camera towards object's position
	void	lookAt(const Object &obj) { lookAt(obj.getPos()); };

	/* Control presets */

	void	presetFreeFirstPerson(t_UserInput &ui,
		const float maxFOV = 45.0f, const float minFOV = 1.0f);
};

#endif