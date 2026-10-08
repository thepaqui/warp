/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:58:19 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:16:43 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.hpp"

// This tells OpenGL that the window has been resized and to thus update
// the rendering viewport to reflect the new window dimensions
// A smaller viewport than the window is not always an error
// It just means OpenGL will only render inside the smaller viewport
// Leaving the remaining space unused but also uncleared so be careful!
void	processWindowResize(GLFWwindow *window, int width, int height)
{
	(void)window;
	winWidth = width;
	winHeight = height;
	glViewport(0, 0, winWidth, winHeight);
}

// This is called every time the mouse is moved
void	processMouse(GLFWwindow* window, double xpos, double ypos)
{
	(void)window;
	mouseX = xpos;
	mouseY = ypos;
}

// This is called every time a scroll is detected
void	processScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	scrollOffsetX += xoffset;
	scrollOffsetY += yoffset;
}
