/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgram.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:28:49 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/10 14:07:21 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderProgram.hpp"

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->_id);
}

// Can throw :
// - ShaderCreationException
// - ShaderCompilationException
// - ShaderProgramCreationException
// - ShaderProgramLinkingException
// - FileReadingException
ShaderProgram::ShaderProgram(const std::string &vertexShaderSourceFile, const std::string &fragmentShaderSourceFile)
{
	std::string		vertexShaderString;
	std::string		fragmentShaderString;

	std::ifstream	vertexShaderIF;
	std::ifstream	fragmentShaderIF;
	// Ensures ifstream objects throw exceptions if failbit or badbit is set
	vertexShaderIF.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderIF.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderIF.open(vertexShaderSourceFile);
		std::stringstream	vertexShaderStream;
		vertexShaderStream << vertexShaderIF.rdbuf();
		vertexShaderIF.close();
		vertexShaderString = vertexShaderStream.str();
	}
	catch (...)
	{
		std::cerr << "[SHADER_PROGRAM] READING VERTEX SHADER FROM " << vertexShaderSourceFile << " FAILED" << std::endl;
		throw FileReadingException();
	}

	try
	{
		fragmentShaderIF.open(fragmentShaderSourceFile);
		std::stringstream	fragmentShaderStream;
		fragmentShaderStream << fragmentShaderIF.rdbuf();
		fragmentShaderIF.close();
		fragmentShaderString = fragmentShaderStream.str();
	}
	catch (...)
	{
		std::cerr << "[SHADER_PROGRAM] READING FRAGMENT SHADER FROM " << fragmentShaderSourceFile << " FAILED" << std::endl;
		throw FileReadingException();
	}

	const GLchar	*vertexShaderSource = vertexShaderString.c_str();
	const GLchar	*fragmentShaderSource = fragmentShaderString.c_str();

	initialize(1, &vertexShaderSource, 1, &fragmentShaderSource);
}

// Can throw ShaderCreationException, ShaderCompilationException,
// ShaderProgramCreationException and ShaderProgramLinkingException
// The "Count" argument is the number of strings in the shader's source
ShaderProgram::ShaderProgram(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource)
{
	initialize(vertexCount, vertexShaderSource, fragmentCount, fragmentShaderSource);
}

void	ShaderProgram::initialize(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource)
{
	// VERTEX SHADER CREATION
	GLuint	vertexShader;
	try
	{
		vertexShader = compileShader(GL_VERTEX_SHADER, vertexCount, vertexShaderSource, NULL);
	}
	catch (ShaderCreationException &err)
	{
		std::cerr << "[SHADER_PROGRAM] VERTEX SHADER CREATION FAILED\n" << std::endl;
		throw ;
	}
	catch (CompilationException &err)
	{
		std::cerr << "[SHADER_PROGRAM] VERTEX SHADER COMPILATION FAILED\n" << std::endl;
		throw ;
	}

	// FRAGMENT SHADER CREATION
	GLuint	fragmentShader;
	try
	{
		fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCount, fragmentShaderSource, NULL);
	}
	catch (ShaderCreationException &err)
	{
		glDeleteShader(vertexShader);
		std::cerr << "[SHADER_PROGRAM] FRAGMENT SHADER CREATION FAILED\n" << std::endl;
		throw ;
	}
	catch (CompilationException &err)
	{
		glDeleteShader(vertexShader);
		std::cerr << "[SHADER_PROGRAM] FRAGMENT SHADER COMPILATION FAILED\n" << std::endl;
		throw ;
	}

	// SHADER PROGRAM CREATION
	this->_id = glCreateProgram();
	if (!this->_id)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		std::cerr << "[SHADER_PROGRAM] SHADER PROGRAM CREATION FAILED\n" << std::endl;
		throw CreationException();
	}
	glAttachShader(this->_id, vertexShader);
	glAttachShader(this->_id, fragmentShader);
	try
	{
		link();
	}
	catch (LinkingException &err)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(this->_id);
		std::cerr << "[SHADER_PROGRAM] SHADER PROGRAM LINKING FAILED\n" << std::endl;
		throw ;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void	ShaderProgram::use()
{
	if (!this->_id)
		return ;
	glUseProgram(this->_id);
}

void	ShaderProgram::setBool(const std::string &name, const bool value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] BOOL UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1i(uni_id, value);
}

void	ShaderProgram::setInt(const std::string &name, const int value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] INT UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1i(uni_id, value);
}

void	ShaderProgram::setFloat(const std::string &name, const float value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] FLOAT UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1f(uni_id, value);
}

void	ShaderProgram::setDouble(const std::string &name, const double value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] DOUBLE UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1d(uni_id, value);
}

void	ShaderProgram::setFloatVec3(const std::string &name, const MATF &vec3)
{
	if (!this->_id)
		return ;
	use();
	if (MATF::isVec3(vec3) == false)
	{
		std::cerr << "[SHADER_PROGRAM] TRIED TO SEND NON-VEC3 AS VEC3 UNIFORM " << name << "\n" << std::endl;
		throw std::invalid_argument("Mismatched matrix sizes when sending as vec3 uniform");
	}
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] FLOAT VEC3 " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform3f(uni_id, vec3.getElem(0), vec3.getElem(1), vec3.getElem(2));
}

void	ShaderProgram::setFloatVec4(const std::string &name, const MATF &vec4)
{
	if (!this->_id)
		return ;
	use();
	if (MATF::isVec4(vec4) == false)
	{
		std::cerr << "[SHADER_PROGRAM] TRIED TO SEND NON-VEC4 AS VEC4 UNIFORM " << name << "\n" << std::endl;
		throw std::invalid_argument("Mismatched matrix sizes when sending as vec4 uniform");
	}
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] FLOAT VEC4 " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform4f(uni_id, vec4.getElem(0), vec4.getElem(1), vec4.getElem(2), vec4.getElem(3));
}

void	ShaderProgram::setFloatMat4(const std::string &name, const float *value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] FLOAT MAT4 " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniformMatrix4fv(uni_id, 1, GL_TRUE, value);
}

void	ShaderProgram::setFloatMat3(const std::string &name, const float *value)
{
	if (!this->_id)
		return ;
	use();
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cerr << "[SHADER_PROGRAM] FLOAT MAT3 " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniformMatrix3fv(uni_id, 1, GL_TRUE, value);
}

// Compiles a shader and returns its ID.
// Throws ShaderCreationException if creation fails.
// Throws ShaderCompilationException if compilation fails.
// Arguments are, in order:
//	- shader type
//	- number of strings passed as source code
//	- source code
//	- array of each string's length. If NULL, NULL-termination is assumed
GLuint	ShaderProgram::compileShader(GLenum type, GLsizei count, const GLchar **source, const GLint *length)
{
	// Shader object ID
	GLuint	shader = glCreateShader(type);
	if (!shader)
	{
		std::cerr << "[SHADER_PROGRAM] ERROR WHILE CREATING SHADER" << std::endl;
		throw ShaderCreationException();
	}

	// Links the shader with its source code
	glShaderSource(shader, count, source, length);

	// Shader compilation
	glCompileShader(shader);

	// Checking if the shader compilation was successful
	GLint	success;
	// Puts into success the compilation status of our shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar	infoLog[512];
		// Retrieves compilation log into infoLog
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		glDeleteShader(shader);
		std::cerr << "[SHADER_PROGRAM] ERROR WHILE COMPILING SHADER\n" << infoLog << std::endl;
		throw CompilationException();
	}

	return shader;
}

// This links the program with its attached shaders
// So you should still attach your shaders beforehand!
// In addition, this checks for errors during linking
// If a linking error occurs, throws a ShaderProgramLinkingException
// and does NOT delete the shader program
void	ShaderProgram::link()
{
	glLinkProgram(this->_id);

	GLint	success;
	glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar	infoLog[512];
		glGetProgramInfoLog(this->_id, 512, NULL, infoLog);
		std::cerr << "[SHADER_PROGRAM] ERROR WHILE LINKING SHADER PROGRAM\n" << infoLog << std::endl;
		throw LinkingException();
	}
}
