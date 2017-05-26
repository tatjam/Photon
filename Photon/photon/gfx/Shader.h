#pragma once

#include "../../dep/glad/glad.h"

#include <string>

#include "../facility/Logger.h"

#include "../Engine.h"

#include <iostream>
#include <fstream>

#include <exception>

namespace ph
{

	class Shader
	{
	public:

		Engine* en;

		GLuint pr;

		void use();

		void load(std::string vpath, std::string fpath);

		Shader(std::string vpath, std::string fpath, Engine* e);
		Shader(Engine* e);
		~Shader();
	};

}

