#include "dep/glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../dep/rang.h"

#include "photon/Engine.h"
#include "photon/gfx/Shader.h"

#include "photon/gfx/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ph;

#define rad(x) { glm::radians(x)}


int main()
{
	Engine engine;
	engine.log = Logger();


	int wwidth = 512;
	int wheight = 512;

	engine.log(INF) << "Starting GLFW context, OpenGL 3.3" << endl;

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(wwidth, wheight, "Hello World!", nullptr, nullptr);
	if (window == nullptr)
	{
		engine.log(FTL) << "Failed to create GLFW window!" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load GL functions
	if (!gladLoadGL())
	{
		engine.log(FTL) << "Could not load GL functions (glad error)!" << endl;
		glfwTerminate();
		return -1;
	}

	engine.log(WIN) << "Created window and GL context! Context info: " << endl;
	engine.log(INF) << (char*)glGetString(GL_VERSION) << "|" << (char*)glGetString(GL_VENDOR) << "|" << (char*)glGetString(GL_RENDERER) << endl; 


	Shader shader(&engine);
	shader.load("res/normal.vert", "res/normal.frag");

	Mesh mesh;
	mesh.vertices.push_back(vVertex({ -0.5, 0, 0 }, { 0, 0, 1 }, { 0, 0 }));
	mesh.vertices.push_back(vVertex({ 0.5, 0, 0 }, { 0, 1, 0 }, { 1, 0 }));
	mesh.vertices.push_back(vVertex({ 0.5, 0.5, 0 }, { 0, 1, 1 }, { 1, 1 }));
	mesh.vertices.push_back(vVertex({ 0, 0.5, 0 }, { 1, 0, 0 }, { 0, 1 }));

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(0);
	mesh.indices.push_back(3);
	mesh.indices.push_back(2);

	mesh.create();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glm::mat4 world;
	glm::mat4 view;
	glm::mat4 proj;

	world = glm::translate(world, { 0, 0, 0 });
	view = glm::translate(world, { 0, 0, -2.5 });
	proj = glm::perspective(glm::radians(80.0f), 1.0f, 0.05f, 100.0f);

	//proj = glm::perspective()

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		world = glm::rotate(world, rad(1.0f), glm::vec3(0, 1, 0));

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mesh.render(world, view, proj, &shader);

		// Swap the screen buffers
		glfwSwapBuffers(window);




	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}