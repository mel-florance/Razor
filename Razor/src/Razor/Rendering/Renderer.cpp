#include "rzpch.h"

#include "Renderer.h"
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"

#include "Razor/Core/Engine.h"

#include "Razor/Scene/Node.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Materials/Material.h"
#include "Razor/Geometry/StaticMesh.h"

#include <glad/glad.h>

namespace Razor
{

	Renderer::Renderer(
		Window* window,
		Engine* engine,
		ScenesManager* scenesManager,
		ShadersManager* shadersManager
	) :
		scenes_manager(scenesManager),
		shaders_manager(shadersManager),
		clear_color(glm::vec4(1.0f, 0.65f, 0.0f, 1.0f)),
		g_buffer(nullptr)
	{
		//forward = new ForwardRenderer(window, engine, scenesManager, shadersManager);
		deferred = new DeferredRenderer(window, engine, scenesManager, shadersManager);

		g_buffer = deferred->getGBuffer();

		internal_shaders[Shader::Type::DEFAULT] = shaders_manager->getShaders()["default"];
		internal_shaders[Shader::Type::LANDSCAPE] = shaders_manager->getShaders()["landscape"];
	}

	Renderer::~Renderer()
	{
		delete forward;
		delete deferred;
	}

	void Renderer::update()
	{
	}

	void Renderer::render()
	{
		processQueue();
	}

	void Renderer::onResize(const glm::vec2& size)
	{
		deferred->onResize(size);
	}

	void Renderer::addRenderTask(std::shared_ptr<Node> node)
	{
		if (node->meshes.size() > 0)
		{
			StaticMesh::List::iterator it = node->meshes.begin();

			for (; it != node->meshes.end(); ++it)
			{
				std::shared_ptr<StaticMesh> mesh = (*it);

				RenderTask task;
				task.category     = RenderCategory::OBJECT;
				task.depth_pass   = mesh->isReceivingShadows();
				task.draw_mode    = mesh->getDrawMode();
				task.index_count  = (unsigned int)mesh->getIndices().size();
				task.vertex_count = (unsigned int)mesh->getVertices().size();
				task.vao          = mesh->getVao();

				task.shader = internal_shaders[Shader::Type::DEFAULT]; // TODO: Get from material.

				tasks.emplace(task);
			}
		}
		
		if (node->landscapes.size() > 0)
		{
			Landscape::List::iterator it = node->landscapes.begin();

			for (; it != node->landscapes.end(); it++)
			{
				std::shared_ptr<StaticMesh> mesh = (*it)->getMesh();

				RenderTask task;
				task.category     = RenderCategory::LANDSCAPE;
				task.depth_pass   = mesh->isReceivingShadows();
				task.draw_mode    = mesh->getDrawMode();
				task.index_count  = (unsigned int)mesh->getIndices().size();
				task.vertex_count = (unsigned int)mesh->getVertices().size();
				task.vao          = mesh->getVao();

				task.shader = internal_shaders[Shader::Type::LANDSCAPE];

				tasks.emplace(task);
			}
		}

		Node::List::iterator it = node->nodes.begin();

		for (; it != node->nodes.end(); ++it)
			addRenderTask(*it);
	}

	bool operator < (const Renderer::RenderTask& lhs, const Renderer::RenderTask& rhs)
	{
		return false;
	}

	void Renderer::processQueue()
	{
		std::vector<std::shared_ptr<Node>>& nodes = scenes_manager->getActiveScene()->getSceneGraph()->getNodes();

		for (auto node : nodes)
			addRenderTask(node);

		deferred->render();

		while (!tasks.empty())
		{
			RenderTask task = tasks.top();

			task.shader->bind();
			task.vao->bind();

			//std::cout << task.shader->getName() << std::endl;

			tasks.pop();
		}
	}

}
