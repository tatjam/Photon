#include "SceneRenderer.h"

namespace ph
{

	void SceneRenderer::resize(int width, int height)
	{
		this->width = width;
		this->height = height;
		fillBuffers(width, height);
	}

	void SceneRenderer::fullscreenQuad()
	{
		glBindVertexArray(quadVAO);
		// GL_TRIANGLE_STRIP saves us from using an EBO
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void SceneRenderer::genCamera(int width, int height)
	{
		view = glm::mat4();
		proj = glm::mat4();

		view = glm::lookAt(camera->pos, camera->target, camera->up);
		proj = glm::perspective(camera->fov.asRadians(), (float)width / (float)height, camera->nearPlane, camera->farPlane);
	}


	void SceneRenderer::uploadLightScene()
	{

		glUniform1i(glGetUniformLocation(deferredShader->pr, "pLightCount"), lightScene->pointLightCount);

		for (int i = 0; i < lightScene->pointLightCount; i++)
		{
			std::string lName = "pointLights[";
			lName.append(std::to_string(i));
			lName += ']';
			glUniform3f(glGetUniformLocation(deferredShader->pr, (lName + ".pos").c_str()),
				lightScene->pointLights[i].pos.x, lightScene->pointLights[i].pos.y, lightScene->pointLights[i].pos.z);
			glUniform3f(glGetUniformLocation(deferredShader->pr, (lName + ".col").c_str()),
				lightScene->pointLights[i].col.r, lightScene->pointLights[i].col.g, lightScene->pointLights[i].col.b);
			glUniform1f(glGetUniformLocation(deferredShader->pr, (lName + ".linear").c_str()),
				lightScene->pointLights[i].linear);
			glUniform1f(glGetUniformLocation(deferredShader->pr, (lName + ".quadratic").c_str()),
				lightScene->pointLights[i].quadratic);
		}

		glUniform3f(glGetUniformLocation(deferredShader->pr, "dirLight.dir"),
			lightScene->dirLight.direction.x, lightScene->dirLight.direction.y, lightScene->dirLight.direction.z);
		glUniform3f(glGetUniformLocation(deferredShader->pr, "dirLight.col"),
			lightScene->dirLight.direction.r, lightScene->dirLight.direction.g, lightScene->dirLight.direction.b);

	}

	void SceneRenderer::geometryPass()
	{
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
	}

	void SceneRenderer::lightPass()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		// Lighting pass, do the lighting and deferred compositing
		glBindFramebuffer(GL_FRAMEBUFFER, pBuffer);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			deferredShader->use();
			glUniform3f(glGetUniformLocation(deferredShader->pr, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
			glUniform3f(glGetUniformLocation(deferredShader->pr, "ambient"),
				lightScene->ambient.r, lightScene->ambient.g, lightScene->ambient.b);
			glUniform1i(glGetUniformLocation(deferredShader->pr, "renderMode"), debugMode);

			uploadLightScene();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPos);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNrm);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlb);



			fullscreenQuad();
		}
	}

	void SceneRenderer::postPass()
	{
		// Last pass, post processing and finally presenting to canvas
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			postShader->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pColor);
			fullscreenQuad();
		}
	}

	void SceneRenderer::render()
	{
		genCamera(width, height);
		geometryPass();
		lightPass();
		postPass();
	}

	void SceneRenderer::fillBuffers(int width, int height)
	{
		bool err = false;

		en->log(INF) << "Generating SceneRenderer gBuffers (GL)" << endlog;

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
			en->log(ERR) << "Error while generating the gBuffers! gBuffer not complete" << endlog;
			err = true;
		}
			
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (!err)
		{
			en->log(WIN) << "Successfully generated gBuffers" << endlog;
		}
		else
		{
			en->log(WRN) << "Could not generate gBuffers properly!" << endlog;
		}

		err = false;

		// pBuffers
		en->log(INF) << "Generating SceneRenderer pBuffers (GL)" << endlog;

		glBindTexture(GL_TEXTURE_2D, pColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindRenderbuffer(GL_RENDERBUFFER, pDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, pBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColor, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			en->log(ERR) << "Error while generating the pBuffers! pBuffer not complete" << endlog;
			err = true;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (!err)
		{
			en->log(WIN) << "Successfully generated pBuffers" << endlog;
		}
		else
		{
			en->log(WRN) << "Could not generate pBuffers properly!" << endlog;
		}

	}

	SceneRenderer::SceneRenderer(std::vector<Drawable*>* dr, int width, int height, 
		Engine* engine, Shader* defShader, Shader* postShader, LightScene* scene, LowCamera* camera)
	{
		this->deferredShader = defShader;
		this->postShader = postShader;

		this->camera = camera;

		this->lightScene = scene;
		en = engine;

		// GBuffer
		glGenTextures(1, &gPos);
		glGenTextures(1, &gNrm);
		glGenFramebuffers(1, &gBuffer);
		glGenRenderbuffers(1, &gDepth);
		glGenTextures(1, &gAlb);

		// PBuffer
		glGenFramebuffers(1, &pBuffer);
		glGenTextures(1, &pColor);
		glGenRenderbuffers(1, &pDepth);



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

		defShader->use();

		glUniform1i(glGetUniformLocation(defShader->pr, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(defShader->pr, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(defShader->pr, "gAlbedo"), 2);

		postShader->use();
		glUniform1i(glGetUniformLocation(postShader->pr, "hdrBuffer"), 0);

	}


	SceneRenderer::~SceneRenderer()
	{
	}

}
