/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserInput.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 23:41:19 by thepaqui          #+#    #+#             */
/*   Updated: 2026/03/06 18:04:14 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERINPUT_HPP
# define USERINPUT_HPP

# include "window.hpp"

// This enforces tight packing of structures
# pragma pack(push, 1)

typedef struct Key
{
	bool	current{false};
	bool	previous{false};
}				t_Key;

typedef struct	UserInput
{
	// Letters
	t_Key	keyA;
	t_Key	keyB;
	t_Key	keyC;
	t_Key	keyD;
	t_Key	keyE;
	t_Key	keyF;
	t_Key	keyG;
	t_Key	keyH;
	t_Key	keyI;
	t_Key	keyJ;
	t_Key	keyK;
	t_Key	keyL;
	t_Key	keyM;
	t_Key	keyN;
	t_Key	keyO;
	t_Key	keyP;
	t_Key	keyQ;
	t_Key	keyR;
	t_Key	keyS;
	t_Key	keyT;
	t_Key	keyU;
	t_Key	keyV;
	t_Key	keyW;
	t_Key	keyX;
	t_Key	keyY;
	t_Key	keyZ;
	// Numbers
	t_Key	key0;
	t_Key	key1;
	t_Key	key2;
	t_Key	key3;
	t_Key	key4;
	t_Key	key5;
	t_Key	key6;
	t_Key	key7;
	t_Key	key8;
	t_Key	key9;
	// Keypad
	t_Key	keyKP0;
	t_Key	keyKP1;
	t_Key	keyKP2;
	t_Key	keyKP3;
	t_Key	keyKP4;
	t_Key	keyKP5;
	t_Key	keyKP6;
	t_Key	keyKP7;
	t_Key	keyKP8;
	t_Key	keyKP9;
	t_Key	keyKPDecimal;
	t_Key	keyKPDivide;
	t_Key	keyKPMultiply;
	t_Key	keyKPSubtract;
	t_Key	keyKPAdd;
	t_Key	keyKPEnter;
	t_Key	keyKPEqual;
	// Other printables
	t_Key	keySP;
	t_Key	keyApostrophe;
	t_Key	keyComma;
	t_Key	keyMinus;
	t_Key	keyPeriod;
	t_Key	keySlash;
	t_Key	keySemicolon;
	t_Key	keyEqual;
	t_Key	keyLBracket;
	t_Key	keyRBracket;
	t_Key	keyBackslash;
	t_Key	keyBackTick;
	// Controls
	t_Key	keyEsc;
	t_Key	keyEnter;
	t_Key	keyTab;
	t_Key	keyBackspace;
	t_Key	keyInsert;
	t_Key	keyDelete;
	t_Key	keyRA; // Right Arrow
	t_Key	keyLA; // Left Arrow
	t_Key	keyDA; // Down Arrow
	t_Key	keyUA; // Up Arrow
	t_Key	keyPageUp;
	t_Key	keyPageDown;
	t_Key	keyHome;
	t_Key	keyEnd;
	t_Key	keyCapsLock;
	t_Key	keyScrollLock;
	t_Key	keyNumLock;
	t_Key	keyPrintScreen;
	t_Key	keyPause;
	// Function keys
	t_Key	keyF1;
	t_Key	keyF2;
	t_Key	keyF3;
	t_Key	keyF4;
	t_Key	keyF5;
	t_Key	keyF6;
	t_Key	keyF7;
	t_Key	keyF8;
	t_Key	keyF9;
	t_Key	keyF10;
	t_Key	keyF11;
	t_Key	keyF12;
	t_Key	keyF13;
	t_Key	keyF14;
	t_Key	keyF15;
	t_Key	keyF16;
	t_Key	keyF17;
	t_Key	keyF18;
	t_Key	keyF19;
	t_Key	keyF20;
	t_Key	keyF21;
	t_Key	keyF22;
	t_Key	keyF23;
	t_Key	keyF24;
	t_Key	keyF25;
	// Modifiers
	t_Key	keyLShift;
	t_Key	keyLCtrl;
	t_Key	keyLAlt;
	t_Key	keyLSuper;
	t_Key	keyRShift;
	t_Key	keyRCtrl;
	t_Key	keyRAlt;
	t_Key	keyRSuper;
	t_Key	keyMenu;
	// Mouse
	bool	firstMouse{true};
	float	lastMouseX{static_cast<float>(winWidth / 2)};
	float	lastMouseY{static_cast<float>(winHeight / 2)};
	float	mouseOffsetX{0.0f};
	float	mouseOffsetY{0.0f};
	float	scrollOffsetX{0.0f};
	float	scrollOffsetY{0.0f};
	float	lastScrollOffsetX{0.0f};
	float	lastScrollOffsetY{0.0f};
}				t_UserInput;

# pragma pack(pop)

typedef enum	e_KeyState
{
	KEY_HELD_RELEASED = 0,
	KEY_JUST_RELEASED = 1,
	KEY_HELD_PRESSED = 2,
	KEY_JUST_PRESSED = 3
}				t_KeyState;

bool	closeWindowOnEscape(GLFWwindow *window, t_UserInput &UserInputs);
void	processKeyboardInputs(GLFWwindow *window, t_UserInput &UserInputs);
void	processMouseInputsFP(t_UserInput &UserInputs) noexcept;
void	processScrollInputs(t_UserInput &UserInputs) noexcept;

t_KeyState	getKeyState(const t_Key &key) noexcept;
bool	isKeyJustPressed(const t_Key &key) noexcept;
bool	isKeyJustReleased(const t_Key &key) noexcept;
bool	isKeyHeldPressed(const t_Key &key) noexcept;
bool	isKeyHeldReleased(const t_Key &key) noexcept;
bool	isKeyPressed(const t_Key &key) noexcept;
bool	isKeyReleased(const t_Key &key) noexcept;

#endif