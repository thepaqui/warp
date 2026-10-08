/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:32:14 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/06 17:59:13 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERMANAGER_HPP
# define SHADERMANAGER_HPP

class ShaderManager;

# include "IlluminationModels.hpp"
# include "ShaderInfo.hpp"
# include "ShaderProgram.hpp"
# include <map>
# include <vector>
# include <exception>
# include <filesystem>
# include <fstream>

// Manages all shader programs used in the application
// Provides default shaders for illumination models 0, 1 and 2 only
// Allows creation and use of custom shader programs identified by an ID
class ShaderManager
{
private	:
	std::map<ShaderInfo, ShaderProgram*>	_shaderPrograms;

	size_t	_customShadersCount = 0;
	std::map<size_t, IlluminationModelTraits>	_customShaderIlluminationTraits;

	std::string	_templateDirPath;
	std::string	_shaderDirPath;

public	:
	ShaderManager() = delete;
	ShaderManager(const ShaderManager &other) = delete;
	ShaderManager(const std::string &templateDirPath, const std::string &shaderDirPath);
	~ShaderManager();

	ShaderManager&	operator=(const ShaderManager &other) = delete;

	ShaderProgram&	getShaderProgram(const ShaderInfo &info) const;
	bool	hasShaderProgram(const ShaderInfo &info) const;
	void	addShaderProgram(const ShaderInfo &info);

	size_t	addCustomShaderTemplate(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, IlluminationModelTraits illumTraits);
	bool	doesCustomShaderUseNormalMatrix(const size_t customShaderId) const;
	bool	doesCustomShaderUseAmbient(const size_t customShaderId) const;
	bool	doesCustomShaderUseDiffuse(const size_t customShaderId) const;
	bool	doesCustomShaderUseSpecular(const size_t customShaderId) const;
	bool	doesCustomShaderUseEmission(const size_t customShaderId) const;
	bool	doesCustomShaderUseShininess(const size_t customShaderId) const;
	bool	doesCustomShaderUseCamPos(const size_t customShaderId) const;

	/* Exceptions */
	class ShaderTemplateNotFoundException : public std::exception
	{
	private:
		std::string	_msg;
	public:
		ShaderTemplateNotFoundException(const std::string &name) : _msg("Shader template " + name + " not found") {}
		const char* what() const noexcept override
		{
			return _msg.c_str();
		}
	};

	class ShaderTemplateCreationFailedException : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "Shader template creation failed";
		}
	};

	class ShaderCreationFailedException : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "Shader creation failed";
		}
	};

	class ShaderProgramCreationFailedException : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "Shader program creation failed";
		}
	};

	class ShaderProgramNotFoundException : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "Shader program not found";
		}
	};
};

#endif