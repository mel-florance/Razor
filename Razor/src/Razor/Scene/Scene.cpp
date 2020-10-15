#include "rzpch.h"
#include "Scene.h"

#include "Razor/Rendering/ForwardRenderer.h"

namespace Razor 
{

	Scene::Scene(const std::string& name) : 
		active(false),
		name(name),
		graph(nullptr),
		active_camera(nullptr),
		lights({}),
		particle_systems({})
	{
		graph = new SceneGraph();
	}

	Scene::~Scene()
	{
		delete graph;
	}

	size_t Scene::getInstancesSize()
	{
		size_t sum = 0;

		for (auto n : graph->getNodes())
			for (auto m : n->meshes)
				sum += m->getInstances().size();
	
		return sum;
	}

	void Scene::addLight(std::shared_ptr<Light> light, Light::Type light_type)
	{
		lights.push_back(light);

		/*switch (light_type)
		{
			case Light::Type::DIRECTIONAL:
				ForwardRenderer::incrementDirectionalLights();
				break;
			case Light::Type::POINT:
				ForwardRenderer::incrementPointLights();
				break;
			case Light::Type::SPOT:
				ForwardRenderer::incrementSpotLights();
				break;
		}*/
	}

	void Scene::removeLight(std::shared_ptr<Light> light)
	{
		auto it = std::find(lights.begin(), lights.end(), light);

		if (it != lights.end())
		{
			switch (light->getType())
			{
				case Light::Type::DIRECTIONAL:
					ForwardRenderer::decrementDirectionalLights();
					break;
				case Light::Type::POINT:
					ForwardRenderer::decrementPointLights();
					break;
				case Light::Type::SPOT:
					ForwardRenderer::decrementSpotLights();
					break;
			}

			light.reset();
			lights.erase(it);
		}
	}

}
