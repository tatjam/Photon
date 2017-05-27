#include "dep/glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../dep/rang.h"

#include "photon/Engine.h"
#include "photon/gfx/Shader.h"

#include "photon/gfx/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "photon/gfx/SceneRenderer.h"

using namespace ph;

#define rad glm::radians
#define tos std::to_string

Engine* gl;

void handleInput(GLFWwindow* win, int key, int scanmode, int action, int mode)
{
	gl->inputHandler(win, key, scanmode, action, mode);
}

int main()
{

	bool wireframe = false;

	float updateInterval = 1.0f / 400.0f;

	Engine engine;
	engine.log = Logger();
	gl = &engine;

	int wwidth = 1024;
	int wheight = 780;

	engine.log(INF) << "Starting GLFW context, OpenGL 3.3" << endlog;

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
		engine.log(FTL) << "Failed to create GLFW window!" << endlog;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, handleInput);

	glfwMakeContextCurrent(window);

	// Load GL functions
	if (!gladLoadGL())
	{
		engine.log(FTL) << "Could not load GL functions (glad error)!" << endlog;
		glfwTerminate();
		return -1;
	}

	engine.log(WIN) << "Created window and GL context! Context info: " << endlog;
	engine.log(INF) << 
		(char*)glGetString(GL_VERSION) << "|" << 
		(char*)glGetString(GL_VENDOR) << "|" << (char*)glGetString(GL_RENDERER) << endlog; 


	Shader shader(&engine);
	shader.load("res/shader/normal.vert", "res/shader/normal.frag");

	Shader fullshader(&engine);
	fullshader.load("res/shader/fullscreen.vert", "res/shader/fullscreen.frag");

	Model m = Model(&engine, &shader);
	m.load("res/test.obj");

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glm::mat4 world;
	glm::mat4 view;
	glm::mat4 proj;

	world = glm::translate(world, { 0, 0, 0 });
	view = glm::translate(world, { 2, 0, -4.5 });
	proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.05f, 250.0f);

	//proj = glm::perspective()

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	float dt = 0.0f;
	float rdt = 0.0f;

	glfwSwapInterval(0);

	double prevTime = 0.0;

	float timer = 0.0f;
	float slowTimer = 0.0f;
	// Game loop

	m.view = view;
	m.proj = proj;

	std::vector<Drawable*> dr;

	dr.push_back(&m);

	SceneRenderer renderer = SceneRenderer(&dr, width, height, &engine, &fullshader);

	while (!glfwWindowShouldClose(window))
	{		

		world = glm::rotate(world, rad(25.0f) * dt, { 0, 1, 0 });
		world = glm::rotate(world, rad(25.0f) * dt, { 1, 0, 0 });

		m.world = world;

		prevTime = glfwGetTime();

		glfwPollEvents();

		if (engine.keyPress(GLFW_KEY_W))
		{
			renderer.wireframe = !renderer.wireframe;
		}

		engine.postUpdate(dt);


		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render();

		// Swap the screen buffers
		glfwSwapBuffers(window);

		dt = glfwGetTime() - prevTime;

		timer += dt;


		if (timer >= 0.5f)
		{
			std::string dts = "FPS: ";
			dts.append(std::to_string(1.0f / dt));

			glfwSetWindowTitle(window, dts.c_str());
			timer = 0.0f;
		}

	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}