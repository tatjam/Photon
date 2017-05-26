#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace ph
{

	class Transform
	{
	public:

		glm::vec3 pos;
		glm::vec3 scale;
		glm::quat rot;

		// Only updated when buildMatrix is called!
		glm::mat4 mat;

		glm::mat4 buildMatrix();

		void rotEuler(glm::vec3 euler);

		glm::vec3 getEuler();

		void move(glm::vec3 mov);
		void rotate(glm::vec3 rot);

		Transform();
		~Transform();
	};

}