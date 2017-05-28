#pragma once

#include "../Drawable.h"
#include "RendererObjects.h"
#include <vector>

#include "../../util/Angle.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ph
{
	
	// GBuffer Info:
	// gPos: Position Data -> (RGB)
	// gNrm: Normal Data -> (RGB) 
	// gAlb: Albedo/Spec Data -> (RGB: Color) (A: Albedo)
	// gDepth is the depth buffer but is not really part of the Deffered Renderer

	// Not really used by the user, but is used by SceneManager under the hood
	class SceneRenderer
	{

	private:

		GLuint gBuffer, gPos, gNrm, gAlb, gDepth; // More to be added

		GLuint pBuffer, pColor, pDepth;

		GLuint quadVBO, quadVAO;

		void fullscreenQuad();

		void uploadLightScene();

		// Render scene into gBuffers
		inline void geometryPass();

		// Render gBuffers into color buffer 
		// and apply lighting
		inline void lightPass();

		// Render color buffer into screen using
		// post processing shader
		inline void postPass();

		inline void genCamera(int width, int height);

		void fillBuffers(int width, int height);

		glm::mat4 view, proj;

		int width, height;

	public:


		LowCamera* camera;

		LightScene* lightScene;



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

		// This shader is the deferred rendering shader
		Shader* deferredShader;

		// This shader is the post processing shader
		Shader* postShader;

		std::vector<Drawable*>* drawables;

	


		// Must be called on window resize!
		void resize(int width, int height);

		void render();

		SceneRenderer(std::vector<Drawable*>* dr, int w, int h, Engine* engine, 
			Shader* defShader, Shader* postShader, LightScene* scene, LowCamera* camera);
		~SceneRenderer();
	};

}

