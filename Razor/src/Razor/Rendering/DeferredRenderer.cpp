#include "rzpch.h"
#include <glad/glad.h>
#include "DeferredRenderer.h"
#include "Razor/Buffers/GBuffer.h"
#include "Razor/Buffers/TextureAttachment.h"
#include "Razor/Buffers/FrameBuffer.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Materials/Shader.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Spot.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Rendering/PBRPipeline.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Materials/EnvironmentTexture.h"
#include <glm/gtx/string_cast.hpp>
#include "Razor/Core/Utils.h"

namespace Razor
{
	int DeferredRenderer::num_directional_lights = 1;
	int DeferredRenderer::num_point_lights = 1;
	int DeferredRenderer::num_spot_lights = 1;

	Shader* DeferredRenderer::deferred_shader = nullptr;
	ShadersManager* DeferredRenderer::shadersManager = nullptr;

	DeferredRenderer::DeferredRenderer(
		Window* window,
		Engine* engine, 
		ScenesManager* scenesManager,
		ShadersManager* shaders_manager
	) :
		window(window),
		engine(engine),
		scenesManager(scenesManager),
		g_buffer(nullptr),
		geometry_shader(nullptr),
		pbr_pipeline(nullptr),
		render_size(glm::ivec2(1920, 1080)),
		quad(nullptr)
	{
		shadersManager = shaders_manager;

		//ironAlbedoMap = new Texture("./data/pbr/gold/albedo.png", false);
		//ironNormalMap = new Texture("./data/pbr/gold/normal.png", false);
		//ironMetallicMap = new Texture("./data/pbr/gold/metallic.png", false);
		//ironRoughnessMap = new Texture("./data/pbr/gold/roughness.png", false);
		//ironAOMap = new Texture("./data/pbr/gold/ao.png", false);

		//glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		setup_framebuffers();
		setup_deferred_shaders();

		pbr_pipeline = new PBRPipeline(shaders_manager);

		EnvironmentTexture* env_texture = new EnvironmentTexture("./data/shanghai_bund_2k.hdr");
		pbr_pipeline->setEnvironmentTexture(env_texture);
		pbr_pipeline->updateEnvironment();

		quad = new Quad();
		sphere = new UVSphere();
		cube = new Cube();
		cube->setCulling(false);

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::vec3 pos = glm::vec3(
				Utils::randomf(-10.0f, 10.0f),
				Utils::randomf(-20.0f, 5.0f),
				Utils::randomf(-10.0f, 10.0f)
			);

			float rColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			float gColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			float bColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			glm::vec3 col = glm::vec3(200.0f, 200.0f, 200.0f);
			
			std::shared_ptr<Razor::Node> node_point = std::make_shared<Razor::Node>();
			node_point->name = "Point Light " + std::to_string(i);
			std::shared_ptr<Point> point_light = std::make_shared<Razor::Point>(scenesManager->getActiveScene()->getActiveCamera(), pos);
			point_light->setDiffuse(col);
			point_light->setIntensity(120.0f);
			node_point->lights.push_back(point_light);

			scenesManager->getActiveScene()->addLight(point_light, Razor::Light::Type::POINT);
			scenesManager->getActiveScene()->getSceneGraph()->addNode(node_point);
		}
	}

	DeferredRenderer::~DeferredRenderer()
	{
		delete pbr_pipeline;
		delete g_buffer;
	}

	void DeferredRenderer::setup_deferred_shaders()
	{
		geometry_shader = shadersManager->getShader("g_buffer");
		deferred_shader = shadersManager->getShader("deferred");
		fbo_debug_shader = shadersManager->getShader("fbo_debug");

		fbo_debug_shader->bind();
		fbo_debug_shader->setUniform1i("fboAttachment", 0);

		deferred_shader->bind();
		deferred_shader->setUniform1i("gPosition", 0);
		deferred_shader->setUniform1i("gNormal", 1);
		deferred_shader->setUniform1i("gAlbedoSpec", 2);
	}

	void DeferredRenderer::setup_framebuffers()
	{
		g_buffer = new GBuffer(render_size);
	}

	void DeferredRenderer::render()
	{
		//geometryPass();
		lightingPass();
	}

	void DeferredRenderer::bindLights(Shader* shader, const std::vector<std::shared_ptr<Light>>& lights)
	{
		if (shader != nullptr)
		{
			std::string paramType;
			unsigned int sIdx = 0, dIdx = 0, pIdx = 0;

			for (auto light : lights)
			{
				if (light->getType() == Light::Type::POINT)
				{
					std::shared_ptr<Point> point = std::dynamic_pointer_cast<Point>(light);
					paramType = "point_lights";

					shader->setUniform3f(formatParamName(paramType, pIdx, "position"), point->getPosition());
					//shader->setUniform1f(formatParamName(paramType, pIdx, "linear"), point->getConstant());
					//shader->setUniform1f(formatParamName(paramType, pIdx, "quadratic"), point->getQuadratic());
					shader->setUniform3f(formatParamName(paramType, pIdx, "diffuse"), point->getDiffuse());
					//shader->setUniform1f(formatParamName(paramType, pIdx, "intensity"), point->getIntensity());
					//shader->setUniform3f(formatParamName(paramType, pIdx, "ambient"), point->getAmbient());
					//shader->setUniform3f(formatParamName(paramType, pIdx, "specular"), point->getSpecular());

					pIdx++;
				}
			}
		}
	}

	std::string DeferredRenderer::formatParamName(const std::string& type, unsigned int index, const std::string& name)
	{
		std::stringstream str;
		str << type << "[" << index << "]." << name;

		return str.str();
	}

	void DeferredRenderer::geometryPass()
	{
		Camera* camera = scenesManager->getActiveScene()->getActiveCamera();
		SceneGraph::NodeList nodes = scenesManager->getActiveScene()->getSceneGraph()->getNodes();

		glBindFramebuffer(GL_FRAMEBUFFER, g_buffer->getBuffer());
		glViewport(0, 0, render_size.x, render_size.y);

		setClearColor(glm::vec4(1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		geometry_shader->bind();
		geometry_shader->setUniformMat4f("projection", camera->getProjectionMatrix());
		geometry_shader->setUniformMat4f("view", camera->getViewMatrix());

		for (auto node : nodes)
		{
			geometry_shader->setUniformMat4f("model", node->transform.getMatrix());

			for (auto mesh : node->meshes)
			{
				std::shared_ptr<Material> material = mesh->getMaterial();

				if (material != nullptr)
					material->bind(geometry_shader);

				mesh->getVao()->bind();
				mesh->draw();
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DeferredRenderer::lightingPass()
	{
		Camera* camera = scenesManager->getActiveScene()->getActiveCamera();

		glBindFramebuffer(GL_FRAMEBUFFER, g_buffer->getFrame());

		glViewport(0, 0, render_size.x, render_size.y);
		setClearColor(glm::vec4(0.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		glDisable(GL_DEPTH_TEST);
		Transform p;
		p.setPosition(camera->getPosition());
		p.setScale(glm::vec3(300.0f));

		Shader* shader_background = pbr_pipeline->getShaderBackground();
		shader_background->bind();
		shader_background->setUniformMat4f("projection", camera->getProjectionMatrix());
		shader_background->setUniformMat4f("view", camera->getViewMatrix());
		shader_background->setUniformMat4f("model", p.getMatrix());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pbr_pipeline->getEnvCubemap());
		cube->getVao()->bind();
		cube->draw();
		glEnable(GL_DEPTH_TEST);

		Shader* shader_pbr = pbr_pipeline->getShaderPBR();

		shader_pbr->bind();
		shader_pbr->setUniformMat4f("view", camera->getViewMatrix());
		shader_pbr->setUniformMat4f("projection", camera->getProjectionMatrix());
		shader_pbr->setUniform3f("camPos", camera->getPosition());

		bindLights(shader_pbr, scenesManager->getActiveScene()->getLights());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pbr_pipeline->getIrradianceMap());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pbr_pipeline->getPrefilterMap());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, pbr_pipeline->getBrdfLutTexture());

		SceneGraph::NodeList nodes = scenesManager->getActiveScene()->getSceneGraph()->getNodes();

		for (auto node : nodes)
			drawNode(node, shader_pbr, glm::mat4(1.0f));

		//renderSphere();

		/*deferred_shader->bind();

		bindLights(deferred_shader, scenesManager->getActiveScene()->getLights());
		deferred_shader->setUniform3f("viewPos", camera->getPosition());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_buffer->getPosition());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_buffer->getNormal());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, g_buffer->getColor());

		quad->getVao()->bind();
		quad->draw();*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void DeferredRenderer::drawNode(std::shared_ptr<Node> node, Shader* shader, glm::mat4 parent)
	{
		if (node->active)
		{
			glm::mat4 local = parent * node->transform.getMatrix();

			for (auto mesh : node->meshes)
			{
				shader->setUniformMat4f("model", local);
				std::shared_ptr<Material> material = mesh->getMaterial();

				if (material != nullptr) {
					if (material->hasOpacityMap()) {
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}
					
					material->bind(shader);
				}

				mesh->getVao()->bind();
				mesh->draw();

				if (material != nullptr) {
					if (material->hasOpacityMap()) {
						glDisable(GL_BLEND);
					}
				}
			}

			std::vector<std::shared_ptr<Node>>::iterator it = node->nodes.begin();

			for (; it != node->nodes.end(); ++it)
				drawNode(*it, shader, local);
		}
	}

	void DeferredRenderer::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void DeferredRenderer::onResize(const glm::vec2& size)
	{
		render_size = size;
	}

	void DeferredRenderer::clear(int flags)
	{
		if (flags == ClearType::ALL)
			flags = ClearType::COLOR | ClearType::DEPTH | ClearType::STENCIL;

		glClear(flags);
	}

}
