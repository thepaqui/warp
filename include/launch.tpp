/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.tpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 15:44:32 by thepaqui          #+#    #+#             */
/*   Updated: 2026/04/10 18:16:34 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCH_TPP
# define LAUNCH_TPP
# include "launch.hpp"

template<typename Func, typename... Args>
requires std::invocable<Func, GLFWwindow*, Args...>
int	launch(Func func, Args... args)
{
	if (func == NULL)
		return launchError("Render Loop function is NULL", 1);

	// Initializing GLFW
	if (glfwInit() == GLFW_FALSE)
		return launchError("Could not initialize GLFW", 1);
	// Telling GLFW that we are using OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Telling GLFW that we are using OpenGL in core-profile mode (i.e. modern OpenGL)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Creating a window with GLFW
	GLFWwindow	*window = glfwCreateWindow(winWidth, winHeight, "humangl", NULL, NULL);
	if (window == NULL)
		return launchError("Could not create GLFW window", 2);
	// Making the window's context the context of the current thread
	glfwMakeContextCurrent(window);

	// Initializing GLAD
	// glwfGetProcAddress loads all OpenGL function pointers (OS-specific)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return launchError("Could not initialize GLAD", 3);

	// Telling OpenGL the position and size of its viewport
	// The viewport can be smaller than the actual window size
	// Position (0,0) is the lower-left corner
	glViewport(0, 0, winWidth, winHeight);

	// Telling GLFW which function to call every time our main window is resized
	glfwSetFramebufferSizeCallback(window, processWindowResize);

	// Telling GLFW which function to call every time the mouse moves
	glfwSetCursorPosCallback(window, processMouse);
	// Telling GLFW which function to call every time the user scrolls
	glfwSetScrollCallback(window, processScroll);

	// Render loop
	try
	{
		func(window, std::forward<Args>(args)...);
	}
	catch (std::invalid_argument &err)
	{
		return launchError(err.what(), 4);
	}
	catch (std::exception &err)
	{
		return launchError(err.what(), 4);
	}

	glfwTerminate();
	return 0;
}

#endif