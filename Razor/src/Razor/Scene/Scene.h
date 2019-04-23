#pragma once

#include "SceneGraph.h"

namespace Razor {

	class Camera;

	class Scene
	{
	public:
		Scene(const std::string& name);
		~Scene();

		inline std::string& getName() { return name; }
		inline void setName(const std::string& name) { this->name = name; }

		inline Camera* getActiveCamera() const { return this->active_camera; }
		inline void setActiveCamera(Camera* cam) { this->active_camera = cam; }

		inline SceneGraph* getSceneGraph() const { return graph; }

		inline size_t getNodesSize() { return graph->getNodes().size(); }

	private:
		bool active;
		std::string name;
		Camera* active_camera;
		SceneGraph* graph;
	};

}
