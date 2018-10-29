#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Shader.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShaderFile(const std::string& file){
	std::ifstream stream(file);

	enum ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	int type = ShaderType::NONE;
	std::stringstream ss[2];
	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}

	}

	return{ ss[0].str(), ss[1].str() };
}

unsigned int CompileShader(unsigned int type, const std::string& shader_code){
	//Create a shader object
	unsigned int id = glCreateShader(type);
	const char *code = shader_code.c_str();

	//Give your source to the shader object
	glShaderSource(id, 1, &code, NULL); // or you ca use a null (read docs)

	//Compile the shader
	glCompileShader(id);

	//Query for status
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE){
		//Get len of error message
		int log_len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);

		// Retrieve the message
		char* message = new char[log_len];
		glGetShaderInfoLog(id, log_len, &log_len, message);

		std::cout << "Failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex\n" : "fragment\n")
			<< message << std::endl;

		glDeleteShader(id);
		delete message;

		return 0;
	}


	return id;

}

Shader::Shader(const std::string& shader_file)
{
	ShaderProgramSource shaderSrc = ParseShaderFile(shader_file);

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSrc.VertexSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shaderSrc.FragmentSource);

	//Now that I have compiled the shaders it time to add them into a program
	prgm_id = glCreateProgram();

	//Attach the shaders to the program
	glAttachShader(prgm_id, vs);
	glAttachShader(prgm_id, fs);

	//Link shaders to the program
	glLinkProgram(prgm_id);

	glDetachShader(prgm_id, vs);
	glDetachShader(prgm_id, fs);
}

void Shader::Use()
{
	glUseProgram(prgm_id);
}

Shader::~Shader()
{
	glDeleteProgram(prgm_id);
}

void Shader::SetColor(float col1, float col2, float col3, float col4)
{
	int colorUni = glGetUniformLocation(prgm_id, "ourColor");
	glUniform4f(colorUni, col1, col2, col3, col4);
}

void Shader::setUniformInt(const std::string uniString, unsigned int intInput)
{
	glUniform1i(glGetUniformLocation(prgm_id, uniString.c_str()), intInput);
}

void Shader::setUniformMatrix4fv(const std::string uniString, float* trans)
{
	glUniformMatrix4fv(glGetUniformLocation(prgm_id, uniString.c_str()), 1, GL_FALSE, trans);
}


unsigned int Shader::getID(){
	return prgm_id;
}