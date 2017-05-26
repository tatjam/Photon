#include "dep/glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../dep/rang.h"

#include "photon/facility/Logger.h"

using namespace ph;

int main()
{
	Logger l = Logger();

	int wwidth = 512;
	int wheight = 512;

	l(INF) << "Starting GLFW context, OpenGL 3.3";

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
		l(FTL) << "Failed to create GLFW window!";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load GL functions
	if (!gladLoadGL())
	{
		l(FTL) << "Could not load GL functions (glad error)!";
		glfwTerminate();
		return -1;
	}

	l(WIN) << "Created window and GL context!";

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}