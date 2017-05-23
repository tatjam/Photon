#pragma once

#include "../dep/tiny_obj_loader.h"

#include <glm/glm.hpp>

#include <string>


class Model
{
public:

	std::vector<Mesh> meshes;
	// TODO textures & stuff
	void loadObj(std::string path);

};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 nrm;
	glm::vec3 tex;
};

// These are fixed size!
// We don't use a std::vector
struct Mesh
{
	Vertex* verts = NULL;
	int vertCount = 0;
};

