#pragma once

#include "Scene.h"

namespace Razor {

	class ScenesManager
	{
	public:
		ScenesManager();
		~ScenesManager();

		typedef std::vector<std::shared_ptr<Scene>> ScenesArray;

		inline ScenesArray& getScenes() { return scenes; }

		inline void addScene(std::shared_ptr<Scene> scene) {
			scenes.push_back(scene);
		}

		inline std::shared_ptr<Scene> getActiveScene() { return activeScene; }
		inline void setActiveScene(std::shared_ptr<Scene> scene) { activeScene = scene; }

		inline bool removeScene(const std::string& name) {
			for (auto scene : scenes) {
				if (scene->getName() == name) {
					scenes.erase(std::remove(scenes.begin(), scenes.end(), scene), scenes.end());
				}
			}
		}

	private:
		std::shared_ptr<Scene> activeScene;
		ScenesArray scenes;
	};

}

