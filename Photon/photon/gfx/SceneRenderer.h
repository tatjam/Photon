#pragma once

#include "Drawable.h"
#include <vector>

namespace ph
{
	
	// Renders a bunch of drawables and a skybox
	// Also renders debug drawables
	// Handles the framebuffers for you, etc...
	class SceneRenderer
	{

	private:

		GLuint hdrFBO;

		GLuint colorBuff, depthBuff;

		GLuint quadVBO, quadVAO;

		void fullscreenQuad();

	public:

		// Set to true for wireframe rendering
		bool wireframe = false;

		Engine* en;

		// This shader is applied in the final screen quad
		// Basically a post processing shader
		// TODO: Use material instead
		Shader* shader;

		std::vector<Drawable*>* drawables;

		// Must be called on window resize!
		void fillBuffers(int width, int height);

		void render();

		SceneRenderer(std::vector<Drawable*>* dr, int w, int h, Engine* engine, Shader* shader);
		~SceneRenderer();
	};

}

