#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "../../glad/glad.h"

#include "Shader.h"



namespace ph
{


	struct Vertex
	{
		float pX, pY, pZ, nX, nY, nZ, tX, tY;
	};

	struct Mesh
	{
		GLuint VAO, VBO, EBO;


		std::vector<Vertex> vertices;
		std::vector<int> indices;

		// Creates GL stuff from mesh data
		void create();


		void render(glm::mat4 world, glm::mat4 view, glm::mat4 proj, Shader* shader);

	};

	Vertex vVertex(glm::vec3 pos, glm::vec3 nrm, glm::vec2 tex);


	class Model
	{
	public:

	
		// ???
		std::vector<Mesh> meshes;

		void render(glm::mat4 world, glm::mat4 view, glm::mat4 proj);



		Model();
		~Model();
	};

}
