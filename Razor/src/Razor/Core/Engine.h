#pragma once

#include "GameLoop.h"

namespace Razor 
{

	class ForwardRenderer;
	class DeferredRenderer;
	class ScenesManager;
	class SoundsManager;
	class ShadersManager;
	class Window;
	class Application;
	class Event;
	class World;

	class Engine
	{
	public:
		Engine(Application* application);
		~Engine();

		void start();
		void stop();
		void OnEvent(Event& event);
		static void update(GameLoop* loop, Engine* self);
		static void render(GameLoop* loop, Engine* self);

		inline ScenesManager* getScenesManager() { return scenes_manager; }
		inline SoundsManager* getSoundsManager() { return sounds_manager; }
		inline ShadersManager* getShadersManager() { return shaders_manager; }

		inline ForwardRenderer* getForwardRenderer() { return forward_renderer; }
		inline DeferredRenderer* getDeferredRenderer() { return deferred_renderer; }

		inline World* getPhysicsWorld() { return physics_world; }

		inline float getFrameTiming() { return (float)gameLoop->getProfiler()->getReport("frame");  }
		inline float getUpdateTiming() { return (float)gameLoop->getProfiler()->getReport("update");  }
		inline float getRenderTiming() { return (float)gameLoop->getProfiler()->getReport("render");  }
		inline float getSleepTiming() { return (float)gameLoop->getProfiler()->getReport("sleep");  }
		
		inline float getFPS() { return gameLoop->getFps(); }
		inline std::map<int, float>& getFpsArray() { return gameLoop->getFpsList(); }

	private:
		Application* application;
		GameLoop* gameLoop;

		ForwardRenderer* forward_renderer;
		DeferredRenderer* deferred_renderer;

		ScenesManager* scenes_manager;
		SoundsManager* sounds_manager;
		ShadersManager* shaders_manager;

		World* physics_world;
	};

}
