#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../dep/tiny_obj_loader.h"


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
		// Push the matrices

		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "world"), 1, GL_FALSE, glm::value_ptr(world));
		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader->pr, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glm::mat4 Model::getMatrix()
	{
		return world;
	}

	void Model::render(glm::mat4 view, glm::mat4 proj)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (meshes[i].mat != NULL)
			{
				meshes[i].mat->use();
				meshes[i].render(world, view, proj, meshes[i].mat->shader);
			}
			else
			{
				if (defaultMaterial != NULL)
				{
					defaultMaterial->use();
					meshes[i].render(world, view, proj, defaultMaterial->shader);
				}
				else
				{
					// Use engine global shader
				}
			}
		}
	}

	void Model::load(std::string objpath)
	{
		e->log(INF) << "Loading obj file: " << objpath << endlog;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		int flags = 1;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, 
			objpath.c_str(), FileUtil::stripFilename(objpath).c_str(), true);

		bool errors = false;

		if (!err.empty())
		{
			e->log(ERR) << err << endlog;
			errors = true;
		}

		if (!ret)
		{
			e->log(ERR) << "Could not load object file, aborting!" << endlog;
			return;
		}

		// Read obj data

		// Over every shape

		Vertex worker = Vertex();

		for (int i = 0; i < shapes.size(); i++)
		{

			Mesh n = Mesh();

			int indexOffset = 0;

			int vIndex = 0;

			// Loop over every polygon
			for (int p = 0; p < shapes[i].mesh.num_face_vertices.size(); p++)
			{
				int fv = shapes[i].mesh.num_face_vertices[p];

				for (int v = 0; v < fv; v++)
				{
					// Vertex data
					tinyobj::index_t idx = shapes[i].mesh.indices[indexOffset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (!attrib.texcoords.empty())
					{
					    tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					}

					worker.pX = vx; worker.pY = vy; worker.pZ = vz;
					worker.nX = nx; worker.nY = ny; worker.nZ = nz;
					worker.tX = tx; worker.tY = ty;
					n.vertices.push_back(worker);
					n.indices.push_back(vIndex);
					vIndex++;
				}

				indexOffset += fv;


				// Material info is loaded to an LMaterial per mesh
			}


			n.create();
			meshes.push_back(n);
		}

		if (!errors)
		{
			e->log(WIN) << "Obj file loaded successfully!" << endlog;
		}
		else
		{
			e->log(WRN) << "Obj file loaded with errors, check log behind" << endlog;
		}

	}

	Model::Model(Engine* en, Shader* s)
	{
		e = en;
		this->s = s;
	}


	Model::~Model()
	{
	}

}