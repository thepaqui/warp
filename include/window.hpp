/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 18:02:27 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:20:55 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP
# include "glad/glad.hpp"
# include <GLFW/glfw3.h>

inline unsigned int	winWidth = 1200;
inline unsigned int	winHeight = 900;

inline double	mouseX = winWidth / 2;
inline double	mouseY = winHeight / 2;
inline float	mouseSensitivity = 0.15f;
inline double	scrollOffsetX = 0.0;
inline double	scrollOffsetY = 0.0;

/* Events */
void	processWindowResize(GLFWwindow *window, int width, int height);
void	processMouse(GLFWwindow* window, double xpos, double ypos);
void	processScroll(GLFWwindow* window, double xoffset, double yoffset);

#endif