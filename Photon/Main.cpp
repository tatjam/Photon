#include "dep/glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../dep/rang.h"

#include "photon/Engine.h"
#include "photon/gfx/Shader.h"

#include "photon/gfx/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "photon/facility/AssetManager.h"


#include "photon/gfx/renderer/SceneRenderer.h"

#include <chrono>
#include <thread>

using namespace ph;


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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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


	AssetManager assets = AssetManager(&engine);

	std::string n = "res/test.obj";
	engine.log(INF) << assets.makeName(n) << endlog;

	Shader shader(&engine);
	shader.load("res/shader/normal.vert", "res/shader/normal.frag");

	Shader defshader(&engine);
	defshader.load("res/shader/deferred.vert", "res/shader/deferred.frag");

	Shader postshader(&engine);
	postshader.load("res/shader/postprocess.vert", "res/shader/postprocess.frag");

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
	world = glm::scale(world, glm::vec3(0.08f, 0.08f, 0.08f));
	view = glm::translate(world, { 0, -1.0, -4.5 });
	proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.05f, 550.0f);

	//proj = glm::perspective()

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	float dt = 0.0f;
	float rdt = 0.0f;

	float t = 0.0f;

	glfwSwapInterval(0);

	double prevTime = 0.0;

	float timer = 0.0f;
	float slowTimer = 0.0f;
	// Game loop


	std::vector<Drawable*> dr;

	dr.push_back(&m);

	LightScene scene;
	scene.pointLightCount++;
	scene.pointLights[0] = PointLight({ 4,2,0 }, { 1, 1, 1 });

	LowCamera cam;
	cam.pos = { 3, 3, 3 };
	cam.target = { 0, 0, 0 };

	SceneRenderer renderer = SceneRenderer(&dr, width, height, &engine, &defshader, &postshader, &scene, &cam);

	Texture* tex = assets.loadTexture("res/test.png");

	Material mat = Material();
	mat.shader = &shader;
	mat.properties["diffColor"] = v3Property(glm::vec3(1.0f, 0.0f, 0.0f));
	mat.properties["baseTex"] = tProperty(tex);
	renderer.cameraPos = { 0, 0, -4.5 };

	m.defaultMaterial = &mat;

	int prevX = width;
	int prevY = height;

	renderer.resize(width, height);

	while (!glfwWindowShouldClose(window))
	{		
		cam.pos = { sin(t), sin(t), cos(t) };

		int newX, newY;
		glfwGetWindowSize(window, &newX, &newY);

		if (newX != prevX || newY != prevY)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
			renderer.resize(width, height);
			prevX = newX; prevY = newY;
			width = newX; height = newY;
		}
		/*world = glm::rotate(world, rad(25.0f) * dt, { 0, 1, 0 });
		world = glm::rotate(world, rad(25.0f) * dt, { 1, 0, 0 });*/

		m.world = world;

		prevTime = glfwGetTime();

		glfwPollEvents();

		if (engine.keyPress(GLFW_KEY_W))
		{
			renderer.wireframe = !renderer.wireframe;
		}
		if (engine.keyPress(GLFW_KEY_S))
		{
			renderer.debugMode++;
			if(renderer.debugMode >= 5)
			{
				renderer.debugMode = 0;
			}
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

		renderer.render();

		// Swap the screen buffers
		glfwSwapBuffers(window);

		// Frame speed limiting the easy way :P

		std::this_thread::sleep_for(std::chrono::milliseconds(4));

		dt = glfwGetTime() - prevTime;

		timer += dt;

		t += dt;


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