#pragma once

#include "SceneGraph.h"

namespace Razor {

	class Camera;
	class ParticleSystem;

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

		inline void addLight(Light* light) { lights.push_back(light); }
		inline std::vector<Light*>& getLights() { return lights; }

		inline void addCamera(Camera* camera) { cameras.push_back(camera); }
		inline std::vector<Camera*>& getCameras() { return cameras; }

		inline void addParticleSystem(ParticleSystem* system) { particle_systems.push_back(system); }
		inline std::vector<ParticleSystem*>& getParticleSystems() { return particle_systems; }

	private:
		bool active;
		std::string name;
		Camera* active_camera;
		SceneGraph* graph;
		std::vector<Light*> lights;
		std::vector<Camera*> cameras;
		std::vector<ParticleSystem*> particle_systems;
	};

}
