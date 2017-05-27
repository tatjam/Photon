#include "Shader.h"

namespace ph
{

	void Shader::use()
	{
		glUseProgram(pr);
	}

	void Shader::load(std::string vpath, std::string fpath)
	{
		en->log(INF) << "Loading shaders: (" << vpath << ", " << fpath << ")" << endlog;

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		try
		{
			vShaderFile.open(vpath);
			fShaderFile.open(fpath);

			if (vShaderFile.bad() || fShaderFile.bad())
			{
				throw(std::exception("!"));
			}

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::exception e)
		{
			en->log(ERR) << "Could not load shader files" << endlog;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			en->log(ERR) << "Could not compile vertex shader: " << infoLog << endlog;
		}
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			en->log(ERR) << "Could not compile fragment shader: " << infoLog << endlog;
		}
		this->pr = glCreateProgram();
		glAttachShader(this->pr, vertex);
		glAttachShader(this->pr, fragment);
		glLinkProgram(this->pr);
		glGetProgramiv(this->pr, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->pr, 512, NULL, infoLog);
			en->log(ERR) << "Could not link program: " << infoLog << endlog;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		if (success)
		{
			en->log(WIN) << "Loaded shader successfully!" << endlog;
		}

	}


	Shader::Shader(std::string v, std::string f, Engine* e)
	{
		en = e;
		load(v, f);
	}

	Shader::Shader(Engine* e)
	{
		en = e;
	}

	Shader::~Shader()
	{

	}
}