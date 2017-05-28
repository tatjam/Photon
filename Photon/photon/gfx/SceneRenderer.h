#pragma once

#include "Drawable.h"
#include <vector>

// !!Must be the same on the shader!!
#define MAX_PLIGHTS 64

namespace ph
{
	
	struct PointLight
	{
		glm::vec3 pos;
		glm::vec3 col;

		float linear;
		float quadratic;

		PointLight()
		{
			pos = { 0, 0, 0 }; col = { 0,0,0 }; linear = 0.03f; quadratic = 0.03f;
		}

		PointLight(glm::vec3 p, glm::vec3 c)
		{
			pos = p; 
			col = c;
			linear = 0.03f;
			quadratic = 0.03f;
		}

		PointLight(glm::vec3 p, glm::vec3 c, float l, float q)
		{
			pos = p;
			col = c;
			linear = l;
			quadratic = q;
		}
	};

	struct LightScene
	{
		glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };

		PointLight pointLights[MAX_PLIGHTS];
		int pointLightCount = 0;
	};

	// Renders a bunch of drawables and a skybox
	// Also renders debug drawables
	// Handles the framebuffers for you, etc...
	// It does no transparent object ordering, that
	// is the purpose of the SceneManager

	// GBuffer Info:
	// gPos: Position Data -> (RGB)
	// gNrm: Normal Data -> (RGB) 
	// gAlb: Albedo/Spec Data -> (RGB: Color) (A: Albedo)
	// gDepth is the depth buffer but is not really part of the Deffered Renderer
	class SceneRenderer
	{

	private:

		GLuint gBuffer, gPos, gNrm, gAlb, gDepth; // More to be added

		GLuint quadVBO, quadVAO;

		void fullscreenQuad();

	

	public:

		LightScene* lightScene;

		glm::mat4 view, proj;

		glm::vec3 cameraPos;
		glm::vec3 cameraHeading;

		// Set to true for wireframe rendering
		bool wireframe = false;

		// 0 -> Normal rendering
		// 1 -> gBuffer Positions
		// 2 -> gBuffer Normals
		// 3 -> gBuffer Albedo
		// 4 -> gBuffer Specular
		int debugMode = 0;

		Engine* en;

		// This shader is applied in the final screen quad
		// Basically a post processing shader
		// !!IT ALSO DOES DEFERRED LIGHT PASS!!
		Shader* shader;

		std::vector<Drawable*>* drawables;

		// Must be called on window resize!
		void fillBuffers(int width, int height);

		void render();

		SceneRenderer(std::vector<Drawable*>* dr, int w, int h, Engine* engine, Shader* shader, LightScene* scene);
		~SceneRenderer();
	};

}

