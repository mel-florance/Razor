#pragma once

#include "SceneGraph.h"

namespace Razor 
{

	class Camera;
	class ParticleSystem;
	class ForwardRenderer;

	class Scene
	{
	public:
		Scene() :
			active(false),
			name(""),
			graph(nullptr),
			active_camera(nullptr),
			lights({}),
			particle_systems({})
		{}
		Scene(const std::string& name);
		~Scene();

		inline std::string& getName() { return name; }
		inline void setName(const std::string& name) { this->name = name; }

		inline bool& isActive() { return active; }
		inline void setActive(bool value) { active = value; }

		inline Camera* getActiveCamera() const { return this->active_camera; }
		inline void setActiveCamera(Camera* cam) { this->active_camera = cam; }

		inline SceneGraph* getSceneGraph() const { return graph; }
		inline size_t getNodesSize() { return graph->getNodes().size(); }
		size_t getInstancesSize();

		inline std::vector<std::shared_ptr<Light>>& getLights() { return lights; }

		void addLight(std::shared_ptr<Light> light, Light::Type light_type);
		void removeLight(std::shared_ptr<Light> light);

		inline void addCamera(Camera* camera) { cameras.push_back(camera); }
		inline std::vector<Camera*>& getCameras() { return cameras; }

		inline void addParticleSystem(ParticleSystem* system) { particle_systems.push_back(system); }
		inline std::vector<ParticleSystem*>& getParticleSystems() { return particle_systems; }

	private:
		bool active;
		std::string name;
		Camera* active_camera;
		SceneGraph* graph;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<Camera*> cameras;
		std::vector<ParticleSystem*> particle_systems;
	};

}
