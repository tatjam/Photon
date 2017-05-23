#include "photon/dep/glad/glad.h"

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "photon/dep/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "photon/dep/stb_image.h"

int main(void)
{



	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* win = glfwCreateWindow(512, 512, "Hello World!", NULL, NULL);

	int width, height;
	glfwGetFramebufferSize(win, &width, &height);


	glfwMakeContextCurrent(win);


	gladLoadGL();


	glViewport(0, 0, width, height);
	
	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render

		glfwSwapBuffers(win);
	}

	return 0;
}