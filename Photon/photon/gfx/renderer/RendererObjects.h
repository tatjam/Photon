#pragma once

#include <glm/glm.hpp>

#define MAX_PLIGHTS 64

#include "../../util/Angle.h"

namespace ph
{

	// A camera used for glm::lookAt
	struct LowCamera
	{
		glm::vec3 pos = { 0, 0, 0 };
		glm::vec3 target{ 0, 0, 1 };
		glm::vec3 up = { 0, 1, 0 };

		Angle fov = deg(80.0f);
		float nearPlane = 0.05f;
		float farPlane = 500.0f;

		//bool changed = false;
	};

	struct PointLight
	{
		glm::vec3 pos;
		glm::vec3 col;

		float linear;
		float quadratic;

		PointLight()
		{
			pos = { 0, 0, 0 }; col = { 0,0,0 }; linear = 0.03f; quadratic = 0.03f;
		}

		PointLight(glm::vec3 p, glm::vec3 c)
		{
			pos = p;
			col = c;
			linear = 0.03f;
			quadratic = 0.03f;
		}

		PointLight(glm::vec3 p, glm::vec3 c, float l, float q)
		{
			pos = p;
			col = c;
			linear = l;
			quadratic = q;
		}
	};

	struct DirectionalLight
	{
		glm::vec3 direction = { 0, 0, 0 };
		glm::vec3 color = { 0, 0, 0 };

		DirectionalLight(glm::vec3 direction, glm::vec3 color)
		{
			this->direction = direction; this->color = color;
		}

		DirectionalLight()
		{
			this->direction = { 0, 0, 0 }; this->color = { 0, 0, 0 };
		}
	};

	struct LightScene
	{
		glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };

		DirectionalLight dirLight;

		PointLight pointLights[MAX_PLIGHTS];
		int pointLightCount = 0;
	};

}