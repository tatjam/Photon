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

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < drawables->size(); i++)
			{
				drawables->at(i)->render();
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Now render to screen quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuff);
			fullscreenQuad();
		}

		
	}

	void SceneRenderer::fillBuffers(int width, int height)
	{
		bool err = false;

		en->log(INF) << "Generating SceneRenderer renderbuffers (GL)" << endlog;

		// Color (HDR) buffer:
		glBindTexture(GL_TEXTURE_2D, colorBuff);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Depth buffer
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuff);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuff, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuff);
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

	SceneRenderer::SceneRenderer(std::vector<Drawable*>* dr, int width, int height, Engine* engine, Shader* shader)
	{
		this->shader = shader;
		en = engine;

		// Generate our framebuffers
		glGenFramebuffers(1, &hdrFBO);
		glGenTextures(1, &colorBuff);
		glGenRenderbuffers(1, &depthBuff);

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
	}


	SceneRenderer::~SceneRenderer()
	{
	}

}
