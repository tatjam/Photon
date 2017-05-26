#include "Model.h"


namespace ph
{
	Vertex vVertex(glm::vec3 pos, glm::vec3 nrm, glm::vec2 tex)
	{
		Vertex out;
		out.pX = pos.x;
		out.pY = pos.y;
		out.pZ = pos.z;
		out.nX = nrm.x;
		out.nY = nrm.y;
		out.nZ = nrm.z;
		out.tX = tex.x;
		out.tY = tex.y;

		return out;
	}
	void Mesh::create()
	{
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		
		glBindVertexArray(VAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			{
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
				
				// Position (3f)
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);

				// Normal (3f)
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);

				// Uv (2f)
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
				glEnableVertexAttribArray(2);

			}
		}
		glBindVertexArray(0);
	}

	void Mesh::render(glm::mat4 world, glm::mat4 view, glm::mat4 proj, Shader* shader)
	{
		shader->use();

		// Push the matrices

		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "world"), 1, GL_FALSE, glm::value_ptr(world));
		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	Model::Model()
	{
	}


	Model::~Model()
	{
	}

}