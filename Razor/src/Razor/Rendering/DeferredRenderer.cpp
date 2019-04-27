#include "rzpch.h"

#include "DeferredRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Razor/Scene/SceneGraph.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Application/Application.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Terrains/Terrain.h"

namespace Razor {

	DeferredRenderer::DeferredRenderer(Window* window, ScenesManager* scenesManager) :
		scenesManager(scenesManager)
	{
		shadersManager = new ShadersManager();

		diffuseMap = new Texture("./data/checker.png");
		specularMap = new Texture("./data/container_specular.png");

		light = new Light();

		angle = 0.0f;
		deltaTime = 0.0f;

		pointLightPositions = {
		   glm::vec3(-3.0f,  -3.0f,  2.0f),
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

		quad = new Quad();

		framebuffer = new FrameBuffer();
		colorbuffer = framebuffer->addTextureAttachment(framebuffer_size);
		framebuffer->addRenderBufferAttachment(framebuffer_size);


		depthbuffer = new FrameBuffer();
		depth_attachment = depthbuffer->addTextureAttachment(glm::vec2(1024.0f, 1024.0f));

		





		Node* node = new Node();
		Node* nodeP = new Node();
		nodeSphere = new Node();
		Node* nodeT = new Node();

		Cube* cube = new Cube();
		Plane* plane = new Plane();
		Sphere* sphere = new Sphere();
		Terrain* terrain = new Terrain(glm::vec3());
		terrain->generate();
		nodeT->name = "Terrain";
		nodeT->meshes.push_back(terrain->getMesh());

		node->name = "Cube";
		nodeP->name = "Plane";
		node->meshes.push_back(cube);
		nodeP->meshes.push_back(plane);
		nodeSphere->meshes.push_back(sphere);
		nodeSphere->name = "Sphere";

		nodeP->transform.setScale(glm::vec3(6.0f, 1.0f, 6.0f));
		nodeSphere->transform.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

		tps_camera = new TPSCamera(window);
		fps_camera = new FPSCamera(window);
		//camera->position = glm::vec3(3.0f, -2.0, 3.0f);
		//camera->direction = glm::vec3(0.0f);

		std::shared_ptr<Scene> scene = std::make_shared<Scene>("default_scene");
		scenesManager->setActiveScene(scene);
		scene->setActiveCamera(fps_camera);

		scenesManager->getActiveScene()->getSceneGraph()->addNode(node);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeP);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeSphere);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeT);
	}

	DeferredRenderer::~DeferredRenderer()
	{
		
	}
	
	void DeferredRenderer::update(float delta)
	{
		deltaTime = (float)delta;
	}

	void DeferredRenderer::render()
	{


		//Todo: Add all others flags at the begining of this functio and also at the end.

		// Make backface culling available per mesh.
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

		angle += deltaTime;

		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

		framebuffer->bind();
		enableDepthTest();

		clear();

		defaultShader->bind();

		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		defaultShader->setUniform3f("directionalLight.direction", 0.2f, 1.0f, 0.3f);
		defaultShader->setUniform3f("directionalLight.ambient", 0.025f, 0.025f, 0.025f);
		defaultShader->setUniform3f("directionalLight.diffuse", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform3f("directionalLight.specular", 1.0f, 1.0f, 1.0f);

		defaultShader->setUniform3f("pointLights[0].position", pointLightPositions[0]);
		defaultShader->setUniform3f("pointLights[0].ambient", 0.025f, 0.025f, 0.025f);
		defaultShader->setUniform3f("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform1f("pointLights[0].constant", 1.0f);
		defaultShader->setUniform1f("pointLights[0].linear", 0.09f);
		defaultShader->setUniform1f("pointLights[0].quadratic", 0.032f);

		defaultShader->setUniform3f("spotLight.position", 1.0f, -8.0f, 2.0f);
		defaultShader->setUniform3f("spotLight.direction", 0.0f, 1.0f, 0.0f);
		defaultShader->setUniform3f("spotLight.ambient", 0.025f, 0.025f, 0.025f);
		defaultShader->setUniform3f("spotLight.diffuse", 3.0f, 0.5f, 0.5f);
		defaultShader->setUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		defaultShader->setUniform1f("spotLight.constant", 1.0f);
		defaultShader->setUniform1f("spotLight.linear", 0.09f);
		defaultShader->setUniform1f("spotLight.quadratic", 0.032f);
		defaultShader->setUniform1f("spotLight.inner_cutoff", glm::cos(glm::radians(12.5f)));
		defaultShader->setUniform1f("spotLight.outer_cutoff", glm::cos(glm::radians(15.0f)));


		defaultShader->setUniform1f("material.shininess", 32.0f);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->getId());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap->getId());

		for (auto node : scene->getSceneGraph()->getNodes())
			renderNode(&node, glm::mat4(1.0f));

		defaultShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
		defaultShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		defaultShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());

		defaultShader->unbind();
		framebuffer->unbind();
		disableDepthTest();
	}

	void DeferredRenderer::renderNode(Node* node, glm::mat4 parent)
	{
		if (node->name == "Sphere")
		{
			node->transform.setPosition(glm::vec3({3 * std::cos(angle), 0.0f, 3 * std::sin(angle) }));
			node->transform.setRotation(glm::vec3({0.0f, 2 * angle, 0.0f }));
		}
		else if (node->name == "Cube")
		{
			node->transform.setRotation(glm::vec3({ 0.0f, angle, 0.0f }));
		}
		else if (node->name == "Plane")
		{

		}
		else if (node->name == "Terrain")
		{

		}
		else
		{
			//node->transform.setPosition(glm::vec3({ 3 * std::cos(angle), 0.0f, 3 * std::sin(angle) }));
			node->transform.setRotation(glm::vec3({ glm::radians(90.0f), angle, 0.0f }));
		}

		glm::mat4 local = parent * node->transform.getMatrix();
		defaultShader->setUniformMat4f("model", local);

		for (auto& mesh : node->meshes)
		{
			mesh->getVao()->bind();
			mesh->draw();
			mesh->getVao()->unbind();
		}

		for (auto& n : node->nodes)
			renderNode(n, local);
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

	TextureAttachment* DeferredRenderer::getColorBuffer()
	{
		return colorbuffer;
	}

}


