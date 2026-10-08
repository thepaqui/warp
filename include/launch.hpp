/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:58:59 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:45:39 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCH_HPP
# define LAUNCH_HPP

# include "glad/glad.hpp"
# include <GLFW/glfw3.h>
# include <iostream>
# include <string>
# include <concepts>
# include <utility>
# include "window.hpp"

inline int	launchError(std::string msg, int err)
{
	if (err != 1)
		glfwTerminate();
	std::cerr << "[LAUNCH ERROR] " << msg << std::endl;
	return err;
}

template<typename Func, typename... Args>
requires std::invocable<Func, GLFWwindow*, Args...>
int	launch(Func func, Args... args);

# include "launch.tpp"

#endif
