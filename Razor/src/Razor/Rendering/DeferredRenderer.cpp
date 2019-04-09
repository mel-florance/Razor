#include "rzpch.h"

#include "DeferredRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Razor/Scene/SceneGraph.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"


namespace Razor {

	DeferredRenderer::DeferredRenderer(Camera* camera, SceneGraph* sceneGraph) : 
		camera(camera), 
		sceneGraph(sceneGraph)
	{
		shadersManager = new ShadersManager();

		diffuseMap = new Texture("./data/container_diffuse.png");
		specularMap = new Texture("./data/container_specular.png");

		light = new Light();

		angle = 0.0f;
		deltaTime = 0.0f;

		pointLightPositions = {
		   glm::vec3(0.7f,  0.2f,  2.0f),
		   glm::vec3(2.3f, -3.3f, -4.0f),
		   glm::vec3(-4.0f,  2.0f, -12.0f),
		   glm::vec3(0.0f,  0.0f, -3.0f)
		};

		color = { 0.1f, 0.1f, 0.1f, 1.0f };
		framebuffer_size = { 1280, 720 };
	
		defaultShader = shadersManager->createShader("default");
		defaultShader->bind();
		defaultShader->setUniform1i("material.diffuse", 0);
		defaultShader->setUniform1i("material.specular", 1);

		viewportShader = shadersManager->createShader("viewport");

		viewportShader->bind();
		viewportShader->setUniform1i("target", 0);

		quad = new Plane();

		framebuffer = new FrameBuffer();
		colorbuffer = framebuffer->addTextureAttachment(framebuffer_size);
		framebuffer->addRenderBufferAttachment(framebuffer_size);

		Node* node = new Node();
		Cube* cube = new Cube();

		node->meshes.push_back(cube);
		sceneGraph->addNode(node);
	}

	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::update(double delta) 
	{

	}

	void DeferredRenderer::render(double delta)
	{
		angle += 0.0003f;

		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

		framebuffer->bind();
		enableDepthTest();

		clear();

		defaultShader->bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 view = camera->getViewMatrix();

		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];
		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), (float)vp->getSize().x / (float)vp->getSize().y, 0.1f, 100.0f);

		defaultShader->setUniform3f("directionalLight.direction", -0.2f, -1.0f, -0.3f);
		defaultShader->setUniform3f("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
		defaultShader->setUniform3f("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
		defaultShader->setUniform3f("directionalLight.specular", 0.5f, 0.5f, 0.5f);

		defaultShader->setUniform3f("pointLights[0].position", pointLightPositions[0]);
		defaultShader->setUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		defaultShader->setUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		defaultShader->setUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform1f("pointLights[0].constant", 1.0f);
		defaultShader->setUniform1f("pointLights[0].linear", 0.09f);
		defaultShader->setUniform1f("pointLights[0].quadratic", 0.032f);

		defaultShader->setUniform3f("spotLight.position", camera->getPosition());
		defaultShader->setUniform3f("spotLight.direction", camera->getFront());
		defaultShader->setUniform3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		defaultShader->setUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform1f("spotLight.constant", 1.0f);
		defaultShader->setUniform1f("spotLight.linear", 0.09f);
		defaultShader->setUniform1f("spotLight.quadratic", 0.032f);
		defaultShader->setUniform1f("spotLight.inner_cutoff", glm::cos(glm::radians(12.5f)));
		defaultShader->setUniform1f("spotLight.outer_cutoff", glm::cos(glm::radians(15.0f)));

		defaultShader->setUniform3f("viewPosition", camera->getPosition());
		defaultShader->setUniform1f("material.shininess", 32.0f);

		defaultShader->setUniformMat4f("view", view);
		defaultShader->setUniformMat4f("projection", projection);
		defaultShader->setUniformMat4f("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->getId());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap->getId());

		for (auto node : sceneGraph->getNodes())
		{
			for (auto& mesh : node->meshes)
			{
				mesh->getVao()->bind();
				mesh->draw();
				mesh->getVao()->unbind();
			}
		}

		defaultShader->unbind();
		framebuffer->unbind();
		disableDepthTest();


		// TODO: Do some fullscreen with this.

		/*clear(ClearType::COLOR);

		viewportShader->bind();
		quad->getVao()->bind();
		glBindTexture(GL_TEXTURE_2D, colorbuffer->getId());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		quad->getVao()->unbind();
		viewportShader->unbind();*/
	}

	void DeferredRenderer::clear(ClearType type)
	{
		int flags;
		glClearColor(color.x, color.y, color.z, color.w);

		switch (type) {
		default:
		case ClearType::ALL:
			flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			break;
		case ClearType::COLOR:
			flags = GL_COLOR_BUFFER_BIT;
			break;
		case ClearType::DEPTH:
			flags = GL_DEPTH_BUFFER_BIT;
		}

		glClear(flags);
	}

	void DeferredRenderer::enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::setViewport(unsigned int x, unsigned int y, float w, float h)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

}


