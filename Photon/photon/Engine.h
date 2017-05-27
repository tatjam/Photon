#pragma once

#include "facility/Logger.h"

#include <GLFW/glfw3.h>

#define SET_BIT(p,n) ((p) |= (1 << (n)))
#define CLR_BIT(p,n) ((p) &= ~((1) << (n)))
#define GET_BIT(p,n) !!((p) & (1<<(n)))

namespace ph
{
	class Engine
	{
		// 00000ABC 
		// A = Was just released
		// B = Was just pressed
		// C = Actual state
		unsigned char keys[350] = { false };

	public:

	

		Logger log;

		// True if key is down, false if it's not (LOWLEVEL)
		bool keyDown(int key);

		// True if key was pressed this frame, false if not (LOWLEVEL)
		bool keyPress(int key);

		// True if key was just released this frame, false if not (LOWLEVEL)
		bool keyRelease(int key);

		void postUpdate(float dt);

		void inputHandler(GLFWwindow* win, int key, int scanmode, int action, int mode);

		Engine();
		~Engine();
	};
}
