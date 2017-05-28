#include "SceneRenderer.h"

namespace ph
{

	void SceneRenderer::fullscreenQuad()
	{
		glBindVertexArray(quadVAO);
		// GL_TRIANGLE_STRIP saves us from using an EBO
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void SceneRenderer::render()
	{
		// Render scene into our framebuffer
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < drawables->size(); i++)
			{
				drawables->at(i)->render(view, proj);
			}
		}


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		// Lighting pass, do the lighting (TODO)
		// This is also the post processing pass!
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader->use();
			glUniform3f(glGetUniformLocation(shader->pr, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
			glUniform3f(glGetUniformLocation(shader->pr, "ambient"), 
				lightScene->ambient.r, lightScene->ambient.g, lightScene->ambient.b);
			glUniform1i(glGetUniformLocation(shader->pr, "renderMode"), debugMode);

			glUniform1i(glGetUniformLocation(shader->pr, "pLightCount"), lightScene->pointLightCount);

			// LightScene writing
			for (int i = 0; i < lightScene->pointLightCount; i++)
			{
				std::string lName = "pointLights[";
				lName.append(std::to_string(i));
				lName += ']';
				glUniform3f(glGetUniformLocation(shader->pr, (lName + ".pos").c_str()),
					lightScene->pointLights[i].pos.x, lightScene->pointLights[i].pos.y, lightScene->pointLights[i].pos.z);
				glUniform3f(glGetUniformLocation(shader->pr, (lName + ".col").c_str()),
					lightScene->pointLights[i].col.r, lightScene->pointLights[i].col.g, lightScene->pointLights[i].col.b);
				glUniform1f(glGetUniformLocation(shader->pr, (lName + ".linear").c_str()),
					lightScene->pointLights[i].linear);
				glUniform1f(glGetUniformLocation(shader->pr, (lName + ".quadratic").c_str()),
					lightScene->pointLights[i].quadratic);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPos);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNrm);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlb);



			fullscreenQuad();
		}

		
	}

	void SceneRenderer::fillBuffers(int width, int height)
	{
		bool err = false;

		en->log(INF) << "Generating SceneRenderer renderbuffers (GL)" << endlog;

		// Gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		// Position buffer

		glBindTexture(GL_TEXTURE_2D, gPos);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

		// Normal buffer

		glBindTexture(GL_TEXTURE_2D, gNrm);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNrm, 0);

		// Albedo/Spec color buffer

		glBindTexture(GL_TEXTURE_2D, gAlb);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlb, 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &gDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			en->log(ERR) << "Error while generating the buffers! Framebuffer not complete" << endlog;
			err = true;
		}
			
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (!err)
		{
			en->log(WIN) << "Successfully generated buffers" << endlog;
		}

	}

	SceneRenderer::SceneRenderer(std::vector<Drawable*>* dr, int width, int height, 
		Engine* engine, Shader* shader, LightScene* scene)
	{
		this->shader = shader;
		this->lightScene = scene;
		en = engine;

		// GBuffer
		glGenTextures(1, &gPos);
		glGenTextures(1, &gNrm);
		glGenFramebuffers(1, &gBuffer);
		glGenRenderbuffers(1, &gDepth);
		glGenTextures(1, &gAlb);



		fillBuffers(width, height);

		// Build fullscreen quad
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		drawables = dr;

		shader->use();

		glUniform1i(glGetUniformLocation(shader->pr, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shader->pr, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shader->pr, "gAlbedo"), 2);

	}


	SceneRenderer::~SceneRenderer()
	{
	}

}
