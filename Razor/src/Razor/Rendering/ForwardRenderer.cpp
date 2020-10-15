#include "rzpch.h"

#include "ForwardRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Razor/Core/Engine.h"
#include "Razor/Physics/World.h"
#include "Razor/Physics/Bodies/PlanePhysicsBody.h"
#include "Razor/Physics/Bodies/SpherePhysicsBody.h"
#include "Razor/Physics/Bodies/CubePhysicsBody.h"
#include "Razor/Scene/SceneGraph.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Application/Application.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Landscape/Landscape.h"
#include "Razor/Maths/Raycast.h"

#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/Materials/Presets/LandscapeMaterial.h"
#include "Razor/Materials/Presets/AtmosphereMaterial.h"
#include "Razor/Materials/Presets/SkyboxMaterial.h"

#include "Editor/Components/AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/CubemapTexture.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Input/MouseButtons.h"
#include "Razor/Events/Event.h"

#include "Razor/Materials/VideoTexture.h"
#include "Razor/Rendering/BillboardManager.h"

#include "Razor/Network/Http.h"
#include "Razor/Lighting/ShadowCascade.h"

namespace Razor 
{
	using namespace std::string_literals;

	int ForwardRenderer::num_directional_lights = 1;
	int ForwardRenderer::num_point_lights = 1;
	int ForwardRenderer::num_spot_lights = 1;
	Shader* ForwardRenderer::defaultShader = nullptr;
	Shader* ForwardRenderer::landscapeShader = nullptr;
	std::vector<std::shared_ptr<Node>> ForwardRenderer::bounding_boxes = {};
	std::shared_ptr<ColorMaterial> ForwardRenderer::colorMaterial = std::make_shared<ColorMaterial>();
	std::vector<VideoTexture*> ForwardRenderer::video_textures = {};
	glm::vec4 ForwardRenderer::clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	std::unique_ptr<BillboardManager> ForwardRenderer::billboard_manager = std::make_unique<BillboardManager>();
	glm::vec2 ForwardRenderer::geo_offset = glm::vec2(90.0f, 180.0f);
	glm::vec2 ForwardRenderer::geo_coords = glm::vec2(48.866667f, 2.333333f);
	std::string ForwardRenderer::iss_html = std::string();
	glm::vec2 ForwardRenderer::iss_position = glm::vec2(0.0f);
	int ForwardRenderer::outline_width = 2;
	glm::vec4 ForwardRenderer::outline_color = glm::vec4(0.0f, 0.65f, 1.0f, 1.0f);

	std::vector<std::shared_ptr<Node>> ForwardRenderer::line_meshes = {};

	ForwardRenderer::ForwardRenderer(
		Window* window,
		Engine* engine, 
		ScenesManager* scenesManager, 
		ShadersManager* shadersManager
	) :
		debugDepthShader(nullptr),
		depthShader(nullptr),
		gridShader(nullptr),
		lightSpaceMatrix(glm::mat4(1.0f)),
		lightView(glm::mat4(1.0f)),
		particleShader(nullptr),
		viewportShader(nullptr),
		light_bounding_debug(nullptr),
		light_bounding_node(nullptr),
		plane_body(nullptr),
		raycast(nullptr),
		window(window),
		engine(engine),
		scenesManager(scenesManager),
		shadersManager(shadersManager),
		earth_node(nullptr),
		lightViewMatrix(glm::mat4(1.0f)),
		lightProjMatrix(glm::mat4(1.0f)),
		lightProjViewMatrix(glm::mat4(1.0f))
	{
		//setupShaders();

		//angle = 0.0f;
		//deltaTime = 0.0f;

		//quad = std::make_shared<Quad>();

		//framebuffer = new FrameBuffer();
		//framebuffer_size = { 1920, 1080 };
		//colorbuffer = framebuffer->addTextureAttachment(framebuffer_size, false);
		//framebuffer->addRenderBufferAttachment(framebuffer_size);

		//depthbuffer = new FrameBuffer();
		//depth_attachment = depthbuffer->addTextureAttachment(glm::vec2(2048, 2048), true);

		//blurbuffer = new FrameBuffer();
		//blur_attachment = blurbuffer->addTextureAttachment(glm::vec2(2048, 2048));

		//defaultMaterial = std::make_shared<Razor::PhongMaterial>();
		//Texture* tex = new Texture("./data/checker.png");
		//defaultMaterial->setTextureMap(Razor::Material::TextureType::Diffuse, tex->getId());
		//defaultMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

		//// Shadows
		//float near_plane = 0.0f, far_plane = 20.0f;
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		//glShadeModel(GL_FLAT);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glEnable(GL_MULTISAMPLE);
	
		//// Video texture
		//video_texture = new VideoTexture("./data/test.mp4");
		//addVideoTexture(video_texture);

		//Skybox
		//cubemap = new CubemapTexture({
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeRT.png",
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeLF.png",
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeUP.png",
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeDN.png",
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeFT.png",
		//	"./data/skyboxes/purple_nebulae/purple_nebulaeBK.png",
		//});

		//nodeCube = std::make_shared<Node>();
		//nodeCube->name = "SkyBox";
		//cube = std::make_shared<Cube>();
		////cube->setPhysicsEnabled(true);
		//cube->setCulling(false);
	
		//nodeCube->transform.setScale(glm::vec3(500.0f));

		//cube->setPhysicsBody(new CubePhysicsBody(nodeCube.get(), glm::vec3(1.0f, 1.0f, 1.0f)));
		//nodeCube->meshes.push_back(cube);

		//cube->updateBoundings(nodeCube->transform);
		//cube->getBoundingMesh()->setMaterial(ForwardRenderer::getColorMaterial());
		//ForwardRenderer::addBoundingBox(nodeCube);

		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeCube);
		//engine->getPhysicsWorld()->addNode(nodeCube);

		// SkySphere
		//nodeSphere = std::make_shared<Node>();
		//nodeSphere->name = "SkySphere";
		//sphere = std::make_shared<UVSphere>();
		//sphere->setCulling(false);
		//sphere->setIsReceivingShadows(false);
		//sphere->setMaterial(std::make_shared<AtmosphereMaterial>());
		//nodeSphere->meshes.push_back(sphere);
		//nodeSphere->transform.setScale(glm::vec3(900.0f));
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeSphere);

		// Selection buffer
	/*	selection_buffer = new FrameBuffer();
		selection_mask = selection_buffer->addTextureAttachment(glm::vec2(framebuffer_size.x, framebuffer_size.y), false, true);
*/
		// Billboards
		//billboard_manager->addBillboard(nodeDirectional->id, nodeDirectional->transform.getPosition(), "directional_light");

		//light_bounding_node = std::make_shared<Node>();
		//AABB aabb{ -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f };
		//light_bounding_debug = std::make_shared<Bounding>(aabb);
		//light_bounding_debug->setMaterial(ForwardRenderer::getColorMaterial());
		//light_bounding_node->meshes.push_back(light_bounding_debug);
		//addLineMesh(light_bounding_node, 0);
	}

	ForwardRenderer::~ForwardRenderer()
	{
	
	}
	
	void ForwardRenderer::update(float delta)
	{
		deltaTime = (float)delta;
		angle += deltaTime;

		//updateVideoTextures();

		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();
		//EditorViewport* vp = (EditorViewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		//billboard_manager->updatePosition(nodeDirectional, scene->getActiveCamera(), glm::vec4(0, 0, vp->getSize().x, vp->getSize().y));
	}

	void ForwardRenderer::onEvent(Event& event)
	{
		
	}

	void ForwardRenderer::updateLightViewMatrix(const glm::vec3& direction, const glm::vec3& center)
	{
		Transform t;
		glm::vec3 dir = glm::normalize(direction);

		float pitch = asin(-dir.y);
		//lightViewMatrix = glm::rotate(pitch, glm::vec3(0.0f, 1.0f, 0.0f));

		float yaw = atan2(dir.x, dir.z);
		//lightViewMatrix = glm::rotate(-glm::radians(yaw), glm::vec3(1.0f, 0.0f, 0.0f));

		//lightViewMatrix = glm::translate(glm::vec3(-center.x, -center.y, -center.z));

		t.setPosition(-center);
		t.setRotation(glm::vec3(yaw, pitch, 0.0f));

		lightViewMatrix = t.getMatrix();
	}

	void ForwardRenderer::updateLightOrthoMatrix(float width, float height, float length)
	{
		lightProjMatrix[0][0] = 2.0f / width;
		lightProjMatrix[1][1] = 2.0f / height;
		lightProjMatrix[2][2] = -2.0f / length;
		lightProjMatrix[3][3] = 1.0f;

		//float w = width  * 0.5f;
		//float h = height * 0.5f;
		//float l = length * 0.5f;

		//lightProjMatrix = glm::ortho(-w, w, -h, h, -l, l);
	}

	void ForwardRenderer::render()
	{
		//EditorViewport* vp = (EditorViewport*)Application::Get().getEditor()->getComponents()["Viewport"];
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		enableDepthTest();
		glCullFace(GL_FRONT);

		// Depth pass
		depthShader->bind();
		depthShader->setUniformMat4f("model", glm::mat4(1.0f));

		for (auto light : scene->getLights())
		{
			switch (light->getType())
			{
			case Light::Type::DIRECTIONAL:

				std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(light);

				ShadowGenerator* generator = light->getShadowGenerator();
				generator->update(scene->getActiveCamera()->getViewMatrix(), scene);

				std::vector<ShadowCascade*> cascades = generator->getCascades();

				generator->getDepthBuffer()->bind();

				glViewport(0, 0, (GLsizei)generator->getSize().x, (GLsizei)generator->getSize().y);
				glClear(GL_DEPTH_BUFFER_BIT);

				for (auto cascade : cascades)
				{
					depthShader->setUniformMat4f("orthoProjectMatrix", cascade->getOrthoProjMatrix());
					depthShader->setUniformMat4f("lightViewMatrix", cascade->getLightViewMatrix());

					generator->getDepthBuffer()->updateTextureAttachment(true);
					glClear(GL_DEPTH_BUFFER_BIT);

					if (directional->isCastingShadows())
					{
						for (auto node : scene->getSceneGraph()->getNodes()) 
						{
							if (node->meshes.size() > 0)
							{
								if (node->meshes[0]->isReceivingShadows())
									renderNode(depthShader, node, glm::mat4(1.0f), true);
							}
						}
					}
				}

				generator->getDepthBuffer()->unbind();

				break;
			}
		}

		depthShader->unbind();

		//// Shadow blur pass

		//blurShader->bind();
		//blurbuffer->bind();

		//for (auto light : scene->getLights())
		//{
		//	if (light->isCastingShadows())
		//	{
		//		switch (light->getType())
		//		{
		//		case Light::Type::DIRECTIONAL:

		//			std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(light);
		//			ShadowGenerator* generator = directional->getShadowGenerator();

		//			blurShader->setUniform1f("resolution", generator->getSize().x);
		//			blurShader->setUniform1f("radius", generator->getBlurRadius());
		//			blurShader->setUniform2f("dir", generator->getblurDirection());

		//			glActiveTexture(GL_TEXTURE0); 
		//			//video_texture->update();
		//			generator->getDepthAttachment()->bind();

		//			quad->getVao()->bind();
		//			quad->draw();
		//			quad->getVao()->unbind();

		//			generator->getDepthAttachment()->unbind();
		//			break;
		//		}
		//	}
		//}

		//blurShader->unbind();
		//blurbuffer->unbind();

		// Render pass
		
		glCullFace(GL_BACK);
		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);
		framebuffer->bind();
	
		clear();

		// Render skybox

		/*nodeCube->transform.setPosition(scene->getActiveCamera()->getPosition());

		skyboxShader->bind();
		skyboxShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		skyboxShader->setUniformMat4f("proj", scene->getActiveCamera()->getProjectionMatrix());
		skyboxShader->setUniformMat4f("model", nodeCube->transform.getMatrix());
		skyboxShader->setUniform4f("clearColor", clear_color);
		skyboxShader->setUniform1i("cubemap", 0);
		skyboxShader->setUniform2f("limits", -100.0f, 20.0f);

		cubemap->bind(0);

		cube->getVao()->bind();
		cube->draw();

		skyboxShader->unbind();*/

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (scene->getSceneGraph()->getNodes().size() > 0)
		{
			for (auto node : scene->getSceneGraph()->getNodes())
			{
				if (node->name == "Terrain")
				{
					//landscapeShader->bind();
					//node->meshes[0]->getMaterial()->bindLights(landscapeShader, scene->getLights());
					//node->meshes[0]->getMaterial()->bind(landscapeShader);

					//for (auto light : scene->getLights())
					//{
					//	switch (light->getType())
					//	{
					//	case Light::Type::DIRECTIONAL:

					//		std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(light);

					//		if (light->isCastingShadows())
					//		{
					//			glActiveTexture(GL_TEXTURE3); /// TODO: increment unit slot
					//			light->getShadowGenerator()->getDepthAttachment()->bind();
					//			//blur_attachment->bind();
					//		}

					//		landscapeShader->setUniform1f("shadow_bias", light->getShadowGenerator()->getBias());
					//		landscapeShader->setUniform1f("shadow_alpha", light->getShadowGenerator()->getAlpha());
					//		landscapeShader->setUniform1i("pcf_samples", light->getShadowGenerator()->getPcfSamples());
					//		landscapeShader->setUniform3f("lightPos", directional->getDirection());
					//		landscapeShader->setUniformMat4f("lightSpaceMatrix", light->getShadowGenerator()->getLightMatrix(directional->getPosition(), directional->getDirection()));

					//		break;
					//	}
					//}

					//landscapeShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
					//landscapeShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
					//landscapeShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());
			
					//renderNode(landscapeShader, node, glm::mat4(1.0f));

					//landscapeShader->unbind();
				}
				else if (node->name == "SkySphere")
				{
					node->transform.setPosition(glm::vec3(scene->getActiveCamera()->getPosition().x, -40.0f, scene->getActiveCamera()->getPosition().z));

					atmosphereShader->bind();
					atmosphereShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
					atmosphereShader->setUniformMat4f("proj", scene->getActiveCamera()->getProjectionMatrix());
					atmosphereShader->setUniformMat4f("model", node->transform.getMatrix());
					atmosphereShader->setUniform3f("uSunPos", std::dynamic_pointer_cast<Directional>(scene->getLights()[0])->getPosition());
					atmosphereShader->setUniform4f("clearColor", clear_color);
					atmosphereShader->setUniform1f("sun_intensity", scene->getLights()[0]->getIntensity());
					node->meshes[0]->getMaterial()->bind(atmosphereShader);

					node->meshes[0]->getVao()->bind();
					node->meshes[0]->draw();

					atmosphereShader->unbind();
				}
				else 
				{
					defaultShader->bind();
					defaultMaterial->bindLights(defaultShader, scene->getLights());
				
					for (auto light : scene->getLights())
					{
						switch (light->getType())
						{
						case Light::Type::DIRECTIONAL:

							std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(light);
							ShadowGenerator* generator = directional->getShadowGenerator();
							std::vector<ShadowCascade*> cascades = generator->getCascades();

							defaultShader->setUniform1f("shadow_bias", light->getShadowGenerator()->getBias());
							defaultShader->setUniform1f("shadow_alpha", light->getShadowGenerator()->getAlpha());
							defaultShader->setUniform1i("pcf_samples", light->getShadowGenerator()->getPcfSamples());
							defaultShader->setUniform3f("lightPos", directional->getDirection());

							if (light->isCastingShadows())
							{
								for (unsigned int i = 0; i < cascades.size(); i++)
								{
									glActiveTexture(GL_TEXTURE3); /// TODO: increment unit slot
									cascades.at(i)->getDepthTexture()->bind();
									//blur_attachment->bind();
									defaultShader->setUniformMat4f("orthoProjectionMatrix", cascades.at(i)->getOrthoProjMatrix());
									defaultShader->setUniform1f("cascadesFarPlanes", generator->getCascadesSplits()[i]);
									defaultShader->setUniformMat4f("lightViewMatrix", cascades.at(i)->getLightViewMatrix());
								}
							}

							break;
						}
					}

					defaultShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
					defaultShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
					defaultShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());

					renderNode(defaultShader, node, glm::mat4(1.0f));

					defaultShader->unbind();
				}
			}

			//colorbuffer = selection_mask;
		}

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);

		gridShader->bind();

		for (auto& line_mesh : line_meshes)
			renderLineMesh(line_mesh);

		for (auto& box : bounding_boxes)
			renderLineMesh(box, true);

		gridShader->unbind();

		renderOutlines();

		disableDepthTest();

		framebuffer->unbind();
	}

	void ForwardRenderer::renderNode(Shader* shader, std::shared_ptr<Node> node, glm::mat4 parent, bool depth)
	{
		glm::mat4 local = parent * node->transform.getMatrix();


		if (node->name == "Cube_x")
		{
			/*std::shared_ptr<Scene> scene = scenesManager->getActiveScene();
			Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

			unprojected_position = glm::project(
				glm::vec3(0.0f, 0.0f, 0.0f),
				scene->getActiveCamera()->getViewMatrix() * local,
				scene->getActiveCamera()->getProjectionMatrix(),
				glm::vec4(0, 0, vp->getSize().x, vp->getSize().y)
			);*/
		}

		for (auto mesh : node->meshes)
		{
			//if (mesh->getBoundingMesh() != nullptr && mesh->isBoundingBoxVisible()) // Enable for animation later
			//	mesh->updateBoundings(node->transform);

			shader->setUniformMat4f("model", local);

			if (mesh->getMaterial() != nullptr && !depth)
			{
				mesh->getMaterial()->bind(shader);

		/*		if (mesh->getMaterial()->hasDiffuseMap())
				{
					glActiveTexture(GL_TEXTURE0); /// TODO: make dynamic
					video_texture->update();
				}*/
			}

			defaultShader->setUniform1i("instanced", 0);
			mesh->getVao()->bind();
			mesh->draw();

			if (mesh->getInstances().size() > 0)
			{
				defaultShader->setUniform1i("instanced", 1);
				mesh->drawInstances();
			}

			mesh->getVao()->unbind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		for (auto child : node->nodes)
			renderNode(shader, child, local);
	}

	void ForwardRenderer::renderParticleSystems()
	{
	}

	void ForwardRenderer::renderLineMesh(std::shared_ptr<Node> node, bool isBoundingBox)
	{
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		gridShader->setUniformMat4f("model", node->transform.getMatrix());
		gridShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		gridShader->setUniformMat4f("proj", scene->getActiveCamera()->getProjectionMatrix());

		Transform t;
		t.setPosition(node->transform.getPosition());

		for (auto mesh : node->meshes)
		{
			if (!isBoundingBox)
			{
				mesh->getMaterial()->bind(gridShader);
				mesh->getVao()->bind();
				mesh->draw();
				mesh->getVao()->unbind();
			}
			else if(mesh->isBoundingBoxVisible())
			{
				if(mesh->getBoundingMesh()->getMaterial() != nullptr)
				{
					gridShader->setUniformMat4f("model", t.getMatrix());
					mesh->getBoundingMesh()->getMaterial()->bind(gridShader);
					mesh->getBoundingMesh()->getVao()->bind();
					mesh->getBoundingMesh()->draw();
					mesh->getBoundingMesh()->getVao()->unbind();
				}
			}
		}

		for (auto n : node->nodes)
			renderLineMesh(n, isBoundingBox);
	}

	void ForwardRenderer::renderOutlines()
	{
		/*std::shared_ptr<Scene> scene = scenesManager->getActiveScene();
		Selection* selection = Application::Get().getEditor()->getTool<Selection*>("selection");
		EditorViewport* vp = (EditorViewport*)Application::Get().getEditor()->getComponents()["Viewport"];
		Tools* tools = (Tools*)Application::Get().getEditor()->getComponents()["Tools"];
		glm::vec2 vp_size = vp->getSize();

		selection_buffer->bind();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

		gridShader->bind();
		gridShader->setUniform3f("color", glm::vec3(1.0f, 0.65f, 0.0f));
		gridShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		gridShader->setUniformMat4f("proj", scene->getActiveCamera()->getProjectionMatrix());

		glDisable(GL_CULL_FACE);
		for (auto node : selection->getNodes())
		{
			if (node->meshes.size() > 0)
			{
				gridShader->setUniformMat4f("model", node->transform.getMatrix());

				node->meshes[0]->getVao()->bind();
				node->meshes[0]->draw();
				node->meshes[0]->getVao()->unbind();
			}

			for (auto child : node->nodes)
				if (child->meshes.size() > 0)
					renderChildOutlines(glm::mat4(1.0f), child);
		}

		glEnable(GL_CULL_FACE);
		gridShader->unbind();
		selection_mask->unbind();
		selection_buffer->unbind();

		framebuffer->bind();

		outlineShader->bind();
		outlineShader->setUniform1i("mask", 0);
		outlineShader->setUniform2f("pixelSize", glm::vec2(1.0f / vp_size.x , 1.0f / vp_size.y));
		outlineShader->setUniform4f("color", outline_color);
		outlineShader->setUniform1i("width", outline_width);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		selection_mask->bind();

		quad->getVao()->bind();
		quad->draw();
		quad->getVao()->unbind();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		outlineShader->unbind();

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);*/
	}

	void ForwardRenderer::renderChildOutlines(const glm::mat4& parent, std::shared_ptr<Node> node)
	{
		glm::mat4 local = parent * node->transform.getMatrix();
		gridShader->setUniformMat4f("model", local);

		node->meshes[0]->getVao()->bind();
		node->meshes[0]->draw();
		node->meshes[0]->getVao()->unbind();

		for (auto child : node->nodes) 
		{
			child->transform.setScale(glm::vec3(0.004f));

			if (child->meshes.size() > 0)
				renderChildOutlines(glm::mat4(1.0f), child);
		}
	}

	void ForwardRenderer::removeBoundingBox(unsigned int node_id)
	{
		std::vector<std::shared_ptr<Node>>::iterator it = std::find_if(bounding_boxes.begin(), bounding_boxes.end(),
		[=](const std::shared_ptr<Node> n) {
			return n->id == node_id;
		});

		if (it != bounding_boxes.end())
			bounding_boxes.erase(it);
	}

	void ForwardRenderer::updateVideoTextures()
	{
		double t = Application::Get().getEngine()->getGameLoop()->getFrameCounter();

		for (auto texture : video_textures) 
		{
			if (t >= 1.0 / texture->getFps())
				texture->capture();
		}
	}

	void ForwardRenderer::clear(ClearType type)
	{
		int flags;
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		
		switch (type) 
		{
			default:
			case ClearType::ALL:
				flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			case ClearType::COLOR:
				flags = GL_COLOR_BUFFER_BIT;
				break;
			case ClearType::DEPTH:
				flags = GL_DEPTH_BUFFER_BIT;
				break;
			case ClearType::STENCIL:
				flags = GL_STENCIL_BUFFER_BIT;
				break;
		}

		glClear(flags);
	}

	void ForwardRenderer::enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::setViewport(unsigned int x, unsigned int y, float w, float h)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

	void ForwardRenderer::setupShaders()
	{
		outlineShader = shadersManager->addShader("outline", "outline");
		outlineShader->compile();
		outlineShader->link();

		atmosphereShader = shadersManager->addShader("atmosphere", "atmosphere");
		atmosphereShader->compile();
		atmosphereShader->link();

		skyboxShader = shadersManager->addShader("skybox", "skybox");
		skyboxShader->compile();
		skyboxShader->link();

		blurShader = shadersManager->addShader("blur", "blur");
		blurShader->compile();
		blurShader->link();

		viewportShader = shadersManager->addShader("viewport", "viewport");
		viewportShader->compile();
		viewportShader->link();
		viewportShader->bind();
		viewportShader->setUniform1i("target", 0);

		particleShader = shadersManager->addShader("particle", "particle");
		particleShader->compile();
		particleShader->link();

		gridShader = shadersManager->addShader("grid", "grid");
		gridShader->compile();
		gridShader->link();

		depthShader = shadersManager->addShader("depth", "depth");
		depthShader->compile();
		depthShader->link();

		landscapeShader = shadersManager->addShader("landscape", "landscape");
		landscapeShader->defineConstant(Shader::State::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", 2);
		landscapeShader->defineConstant(Shader::State::FRAGMENT, "MAX_POINT_LIGHTS", 1);
		landscapeShader->defineConstant(Shader::State::FRAGMENT, "MAX_SPOT_LIGHTS", 1);
		landscapeShader->replaceConstants();
		landscapeShader->compile();
		landscapeShader->link();

		defaultShader = shadersManager->addShader("default", "default");
		defaultShader->defineConstant(Shader::State::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", 2);
		defaultShader->defineConstant(Shader::State::FRAGMENT, "MAX_POINT_LIGHTS", 1);
		defaultShader->defineConstant(Shader::State::FRAGMENT, "MAX_SPOT_LIGHTS", 1);
		defaultShader->replaceConstants();
		defaultShader->compile();
		defaultShader->link();
	}

	std::vector<std::shared_ptr<Node>> ForwardRenderer::sortRenderGraph()
	{
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();
		std::vector<std::shared_ptr<Node>> sorted;

		for (auto node : scene->getSceneGraph()->getNodes())
			walkRenderGraph(node, sorted);
	
		return sorted;
	}

	void ForwardRenderer::walkRenderGraph(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& sorted)
	{
		sorted.push_back(node);

		for (auto child : node->nodes)
			walkRenderGraph(child, sorted);
	}

	TextureAttachment* ForwardRenderer::getColorBuffer()
	{
		return colorbuffer;
	}

	TextureAttachment* ForwardRenderer::getDepthBuffer()
	{
		return depth_attachment;
	}


	

}
