/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 22:46:08 by thepaqui          #+#    #+#             */
/*   Updated: 2026/07/21 18:39:06 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

void	Camera::calcDir()
{
	const double	pitch_cos = cos(_pitch);
	_dir.setElem(0, (cos(_yaw) * pitch_cos));
	_dir.setElem(1, (sin(_pitch)));
	_dir.setElem(2, (sin(_yaw) * pitch_cos));
}

void	Camera::calcAngle() noexcept
{
	_dir = MATF::normalize(_dir);
	float	newPitch = asin(_dir.getElem(1));
	float	newYaw = atan2(_dir.getElem(2), _dir.getElem(0));
	_pitch = newPitch;
	_yaw = newYaw;
}

void	Camera::setTarget(const MATF &target)
{
	_dir = target - _pos;
	calcAngle();
}

// Flips the worldUp vector
// Analogous to flipping the camera or setting roll to 180
void	Camera::flipView()
{
	_up.setElem(1, _up.getElem(1) * -1.0f);
}

// This takes degrees, not radians!
// Pitch is optional (assumed to be 0)
// This WILL modify the direction vector
void	Camera::setAngle(const float yaw, const float pitch)
{
	_yaw = MATF::degToRad(yaw);
	_pitch = MATF::degToRad(pitch);
	calcDir();
}

// Always call this before moving the camera
void	Camera::setDeltaTime() noexcept
{
	const float	currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
	calcSpeed();
}

// Turns the camera in given direction (setAngle() or setTarget())
void	Camera::lookAt()
{
	_view.lookAt(_pos, _pos + MATF::normalize(_dir), _up);
}

// Preset for a freely moving first person camera
// min/max FOV are optional (assumed to be 1 and 45)
void	Camera::presetFreeFirstPerson(t_UserInput &ui,
	const float maxFOV, const float minFOV)
{
	setDeltaTime();

	processScrollInputs(ui);
	_fov -= ui.scrollOffsetY;
	_fov = std::clamp<float>(_fov, minFOV, maxFOV);

	processMouseInputsFP(ui);
	float	yaw = MATF::radToDeg(getYaw()) + ui.mouseOffsetX;
	yaw -= ui.scrollOffsetX;
	float	pitch = MATF::radToDeg(getPitch()) + ui.mouseOffsetY;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	pitch = std::clamp<float>(pitch, -89.0f, 89.0f);
	setAngle(yaw, pitch);

	if (ui.keyW.current)
		goForward();
	if (ui.keyA.current)
		goLeft();
	if (ui.keyS.current)
		goBackward();
	if (ui.keyD.current)
		goRight();
	if (ui.keySP.current)
		goUp();
	if (ui.keyLShift.current)
		goDown();

	lookAt();
}
