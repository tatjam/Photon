#include "Engine.h"


namespace ph
{
	void Engine::postUpdate(float dt)
	{
		// Previous state updating
		for (int i = 0; i < 350; i++)
		{
			if (keyPress(i))
			{
				CLR_BIT(keys[i], 1);
			}

			if (keyRelease(i))
			{
				CLR_BIT(keys[i], 2);
			}

		}
	}

	void Engine::inputHandler(GLFWwindow* win, int key, int scanmode, int action, int mode)
	{
		
		if (action == GLFW_PRESS)
		{
			SET_BIT(keys[key], 0);
			SET_BIT(keys[key], 1);
		}

		if (action == GLFW_RELEASE)
		{
			CLR_BIT(keys[key], 0);
			SET_BIT(keys[key], 2);
		}

	}

	bool Engine::keyPress(int key)
	{
		return GET_BIT(keys[key], 1);
	}

	bool Engine::keyDown(int key)
	{
		return GET_BIT(keys[key], 0);
	}

	bool Engine::keyRelease(int key)
	{
		return GET_BIT(keys[key], 2);
	}


	Engine::Engine()
	{
	}


	Engine::~Engine()
	{
	}

}