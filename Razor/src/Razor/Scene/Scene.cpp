#include "rzpch.h"
#include "Scene.h"

namespace Razor {

	Scene::Scene(const std::string& name) : 
		active(false),
		name(name)
	{
		graph = new SceneGraph();
	}

	Scene::~Scene()
	{
		delete graph;
	}

}
