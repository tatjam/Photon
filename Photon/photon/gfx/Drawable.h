#pragma once

#include <glm/glm.hpp>
#include "Shader.h"


namespace ph
{

	class Drawable
	{
	public:

		virtual glm::mat4 getMatrix()
		{
			return glm::mat4();
		}

		virtual void render(glm::mat4 view, glm::mat4 proj)
		{

		}

	};

}