/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserInput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:47:10 by thepaqui          #+#    #+#             */
/*   Updated: 2026/03/06 18:04:03 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserInput.hpp"

// When ESC key is pressed, close the window
bool	closeWindowOnEscape(GLFWwindow *window, t_UserInput &UserInputs)
{
	// When ESC key is pressed, window is marked for destruction
	// This will make the render loop exit
	bool escPress = isKeyJustPressed(UserInputs.keyEsc);
	if (escPress)
		glfwSetWindowShouldClose(window, true);
	return escPress;
}

static inline void	setKeyboardInput(GLFWwindow *window,
	const int key, t_Key &toSet)
{
	toSet.previous = toSet.current;
	toSet.current = (glfwGetKey(window, key) == GLFW_PRESS);
}

// Process all keyboard inputs and update UserInputs structure accordingly
void	processKeyboardInputs(GLFWwindow *window, t_UserInput &UserInputs)
{
	setKeyboardInput(window, GLFW_KEY_A, UserInputs.keyA);
	setKeyboardInput(window, GLFW_KEY_B, UserInputs.keyB);
	setKeyboardInput(window, GLFW_KEY_C, UserInputs.keyC);
	setKeyboardInput(window, GLFW_KEY_D, UserInputs.keyD);
	setKeyboardInput(window, GLFW_KEY_E, UserInputs.keyE);
	setKeyboardInput(window, GLFW_KEY_F, UserInputs.keyF);
	setKeyboardInput(window, GLFW_KEY_G, UserInputs.keyG);
	setKeyboardInput(window, GLFW_KEY_H, UserInputs.keyH);
	setKeyboardInput(window, GLFW_KEY_I, UserInputs.keyI);
	setKeyboardInput(window, GLFW_KEY_J, UserInputs.keyJ);
	setKeyboardInput(window, GLFW_KEY_K, UserInputs.keyK);
	setKeyboardInput(window, GLFW_KEY_L, UserInputs.keyL);
	setKeyboardInput(window, GLFW_KEY_M, UserInputs.keyM);
	setKeyboardInput(window, GLFW_KEY_N, UserInputs.keyN);
	setKeyboardInput(window, GLFW_KEY_O, UserInputs.keyO);
	setKeyboardInput(window, GLFW_KEY_P, UserInputs.keyP);
	setKeyboardInput(window, GLFW_KEY_Q, UserInputs.keyQ);
	setKeyboardInput(window, GLFW_KEY_R, UserInputs.keyR);
	setKeyboardInput(window, GLFW_KEY_S, UserInputs.keyS);
	setKeyboardInput(window, GLFW_KEY_T, UserInputs.keyT);
	setKeyboardInput(window, GLFW_KEY_U, UserInputs.keyU);
	setKeyboardInput(window, GLFW_KEY_V, UserInputs.keyV);
	setKeyboardInput(window, GLFW_KEY_W, UserInputs.keyW);
	setKeyboardInput(window, GLFW_KEY_X, UserInputs.keyX);
	setKeyboardInput(window, GLFW_KEY_Y, UserInputs.keyY);
	setKeyboardInput(window, GLFW_KEY_Z, UserInputs.keyZ);

	setKeyboardInput(window, GLFW_KEY_0, UserInputs.key0);
	setKeyboardInput(window, GLFW_KEY_1, UserInputs.key1);
	setKeyboardInput(window, GLFW_KEY_2, UserInputs.key2);
	setKeyboardInput(window, GLFW_KEY_3, UserInputs.key3);
	setKeyboardInput(window, GLFW_KEY_4, UserInputs.key4);
	setKeyboardInput(window, GLFW_KEY_5, UserInputs.key5);
	setKeyboardInput(window, GLFW_KEY_6, UserInputs.key6);
	setKeyboardInput(window, GLFW_KEY_7, UserInputs.key7);
	setKeyboardInput(window, GLFW_KEY_8, UserInputs.key8);
	setKeyboardInput(window, GLFW_KEY_9, UserInputs.key9);

	setKeyboardInput(window, GLFW_KEY_KP_0, UserInputs.keyKP0);
	setKeyboardInput(window, GLFW_KEY_KP_1, UserInputs.keyKP1);
	setKeyboardInput(window, GLFW_KEY_KP_2, UserInputs.keyKP2);
	setKeyboardInput(window, GLFW_KEY_KP_3, UserInputs.keyKP3);
	setKeyboardInput(window, GLFW_KEY_KP_4, UserInputs.keyKP4);
	setKeyboardInput(window, GLFW_KEY_KP_5, UserInputs.keyKP5);
	setKeyboardInput(window, GLFW_KEY_KP_6, UserInputs.keyKP6);
	setKeyboardInput(window, GLFW_KEY_KP_7, UserInputs.keyKP7);
	setKeyboardInput(window, GLFW_KEY_KP_8, UserInputs.keyKP8);
	setKeyboardInput(window, GLFW_KEY_KP_9, UserInputs.keyKP9);
	setKeyboardInput(window, GLFW_KEY_KP_DECIMAL, UserInputs.keyKPDecimal);
	setKeyboardInput(window, GLFW_KEY_KP_DIVIDE, UserInputs.keyKPDivide);
	setKeyboardInput(window, GLFW_KEY_KP_MULTIPLY, UserInputs.keyKPMultiply);
	setKeyboardInput(window, GLFW_KEY_KP_SUBTRACT, UserInputs.keyKPSubtract);
	setKeyboardInput(window, GLFW_KEY_KP_ADD, UserInputs.keyKPAdd);
	setKeyboardInput(window, GLFW_KEY_KP_ENTER, UserInputs.keyKPEnter);
	setKeyboardInput(window, GLFW_KEY_KP_EQUAL, UserInputs.keyKPEqual);

	setKeyboardInput(window, GLFW_KEY_SPACE, UserInputs.keySP);
	setKeyboardInput(window, GLFW_KEY_APOSTROPHE, UserInputs.keyApostrophe);
	setKeyboardInput(window, GLFW_KEY_COMMA, UserInputs.keyComma);
	setKeyboardInput(window, GLFW_KEY_MINUS, UserInputs.keyMinus);
	setKeyboardInput(window, GLFW_KEY_PERIOD, UserInputs.keyPeriod);
	setKeyboardInput(window, GLFW_KEY_SLASH, UserInputs.keySlash);
	setKeyboardInput(window, GLFW_KEY_SEMICOLON, UserInputs.keySemicolon);
	setKeyboardInput(window, GLFW_KEY_EQUAL, UserInputs.keyEqual);
	setKeyboardInput(window, GLFW_KEY_LEFT_BRACKET, UserInputs.keyLBracket);
	setKeyboardInput(window, GLFW_KEY_RIGHT_BRACKET, UserInputs.keyRBracket);
	setKeyboardInput(window, GLFW_KEY_BACKSLASH, UserInputs.keyBackslash);
	setKeyboardInput(window, GLFW_KEY_GRAVE_ACCENT, UserInputs.keyBackTick);

	setKeyboardInput(window, GLFW_KEY_ESCAPE, UserInputs.keyEsc);
	setKeyboardInput(window, GLFW_KEY_ENTER, UserInputs.keyEnter);
	setKeyboardInput(window, GLFW_KEY_TAB, UserInputs.keyTab);
	setKeyboardInput(window, GLFW_KEY_BACKSPACE, UserInputs.keyBackspace);
	setKeyboardInput(window, GLFW_KEY_INSERT, UserInputs.keyInsert);
	setKeyboardInput(window, GLFW_KEY_DELETE, UserInputs.keyDelete);
	setKeyboardInput(window, GLFW_KEY_RIGHT, UserInputs.keyRA);
	setKeyboardInput(window, GLFW_KEY_LEFT, UserInputs.keyLA);
	setKeyboardInput(window, GLFW_KEY_DOWN, UserInputs.keyDA);
	setKeyboardInput(window, GLFW_KEY_UP, UserInputs.keyUA);
	setKeyboardInput(window, GLFW_KEY_PAGE_UP, UserInputs.keyPageUp);
	setKeyboardInput(window, GLFW_KEY_PAGE_DOWN, UserInputs.keyPageDown);
	setKeyboardInput(window, GLFW_KEY_HOME, UserInputs.keyHome);
	setKeyboardInput(window, GLFW_KEY_END, UserInputs.keyEnd);
	setKeyboardInput(window, GLFW_KEY_CAPS_LOCK, UserInputs.keyCapsLock);
	setKeyboardInput(window, GLFW_KEY_SCROLL_LOCK, UserInputs.keyScrollLock);
	setKeyboardInput(window, GLFW_KEY_NUM_LOCK, UserInputs.keyNumLock);
	setKeyboardInput(window, GLFW_KEY_PRINT_SCREEN, UserInputs.keyPrintScreen);
	setKeyboardInput(window, GLFW_KEY_PAUSE, UserInputs.keyPause);

	setKeyboardInput(window, GLFW_KEY_F1, UserInputs.keyF1);
	setKeyboardInput(window, GLFW_KEY_F2, UserInputs.keyF2);
	setKeyboardInput(window, GLFW_KEY_F3, UserInputs.keyF3);
	setKeyboardInput(window, GLFW_KEY_F4, UserInputs.keyF4);
	setKeyboardInput(window, GLFW_KEY_F5, UserInputs.keyF5);
	setKeyboardInput(window, GLFW_KEY_F6, UserInputs.keyF6);
	setKeyboardInput(window, GLFW_KEY_F7, UserInputs.keyF7);
	setKeyboardInput(window, GLFW_KEY_F8, UserInputs.keyF8);
	setKeyboardInput(window, GLFW_KEY_F9, UserInputs.keyF9);
	setKeyboardInput(window, GLFW_KEY_F10, UserInputs.keyF10);
	setKeyboardInput(window, GLFW_KEY_F11, UserInputs.keyF11);
	setKeyboardInput(window, GLFW_KEY_F12, UserInputs.keyF12);
	setKeyboardInput(window, GLFW_KEY_F13, UserInputs.keyF13);
	setKeyboardInput(window, GLFW_KEY_F14, UserInputs.keyF14);
	setKeyboardInput(window, GLFW_KEY_F15, UserInputs.keyF15);
	setKeyboardInput(window, GLFW_KEY_F16, UserInputs.keyF16);
	setKeyboardInput(window, GLFW_KEY_F17, UserInputs.keyF17);
	setKeyboardInput(window, GLFW_KEY_F18, UserInputs.keyF18);
	setKeyboardInput(window, GLFW_KEY_F19, UserInputs.keyF19);
	setKeyboardInput(window, GLFW_KEY_F20, UserInputs.keyF20);
	setKeyboardInput(window, GLFW_KEY_F21, UserInputs.keyF21);
	setKeyboardInput(window, GLFW_KEY_F22, UserInputs.keyF22);
	setKeyboardInput(window, GLFW_KEY_F23, UserInputs.keyF23);
	setKeyboardInput(window, GLFW_KEY_F24, UserInputs.keyF24);
	setKeyboardInput(window, GLFW_KEY_F25, UserInputs.keyF25);

	setKeyboardInput(window, GLFW_KEY_LEFT_SHIFT, UserInputs.keyLShift);
	setKeyboardInput(window, GLFW_KEY_LEFT_CONTROL, UserInputs.keyLCtrl);
	setKeyboardInput(window, GLFW_KEY_LEFT_ALT, UserInputs.keyLAlt);
	setKeyboardInput(window, GLFW_KEY_LEFT_SUPER, UserInputs.keyLSuper);
	setKeyboardInput(window, GLFW_KEY_RIGHT_SHIFT, UserInputs.keyRShift);
	setKeyboardInput(window, GLFW_KEY_RIGHT_CONTROL, UserInputs.keyRCtrl);
	setKeyboardInput(window, GLFW_KEY_RIGHT_ALT, UserInputs.keyRAlt);
	setKeyboardInput(window, GLFW_KEY_RIGHT_SUPER, UserInputs.keyRSuper);
	setKeyboardInput(window, GLFW_KEY_MENU, UserInputs.keyMenu);
}

// For a first-person camera that follows the mouse movements
void	processMouseInputsFP(t_UserInput &UserInputs) noexcept
{
//	std::cout << "MOUSE POS = (" << mouseX << ", " << mouseY << ")" << std::endl;
//	std::cout << "LAST REGISTERED MOUSE POS = (" << UserInputs.FPLastMouseX << ", " << UserInputs.FPLastMouseY << ")" << std::endl;
//	std::cout << "LAST SENT VALUES = (" << UserInputs.FPMouseX << ", " << UserInputs.FPMouseY << ")" << std::endl;

	if (!UserInputs.firstMouse)
	{
		UserInputs.mouseOffsetX = (mouseX - UserInputs.lastMouseX)
			* mouseSensitivity;

		// y mouse pos goes from top to bottom, so we reverse it for pitch
		UserInputs.mouseOffsetY = (UserInputs.lastMouseY - mouseY)
			* mouseSensitivity;
	}
	else if (mouseX != UserInputs.lastMouseX || mouseY != UserInputs.lastMouseY)
		UserInputs.firstMouse = false;

	UserInputs.lastMouseX = mouseX;
	UserInputs.lastMouseY = mouseY;
}

void	processScrollInputs(t_UserInput &UserInputs) noexcept
{
	UserInputs.scrollOffsetX = scrollOffsetX - UserInputs.lastScrollOffsetX;
	UserInputs.scrollOffsetY = scrollOffsetY - UserInputs.lastScrollOffsetY;
	UserInputs.lastScrollOffsetX = scrollOffsetX;
	UserInputs.lastScrollOffsetY = scrollOffsetY;
}

t_KeyState	getKeyState(const t_Key &key) noexcept
{
	if (key.current)
	{
		if (key.previous)
			return KEY_HELD_PRESSED;
		else
			return KEY_JUST_PRESSED;
	}
	else
	{
		if (key.previous)
			return KEY_JUST_RELEASED;
		else
			return KEY_HELD_RELEASED;
	}
}

bool	isKeyJustPressed(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_JUST_PRESSED);
}

bool	isKeyJustReleased(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_JUST_RELEASED);
}

bool	isKeyHeldPressed(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_HELD_PRESSED);
}

bool	isKeyHeldReleased(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_HELD_RELEASED);
}

bool	isKeyPressed(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_JUST_PRESSED || getKeyState(key) == KEY_HELD_PRESSED);
}

bool	isKeyReleased(const t_Key &key) noexcept
{
	return (getKeyState(key) == KEY_JUST_RELEASED || getKeyState(key) == KEY_HELD_RELEASED);
}