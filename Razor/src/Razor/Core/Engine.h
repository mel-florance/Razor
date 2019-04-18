#pragma once

#include "GameLoop.h"

namespace Razor {

	class DeferredRenderer;
	class ScenesManager;
	class Window;
	class Application;
	class Event;

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
		inline DeferredRenderer* getDeferredRenderer() { return deferred_renderer; }
		inline float getFPS() { return gameLoop->getFps(); }
		inline float getFrameTiming() { return (float)gameLoop->getProfiler()->getReport("frame");  }
		inline float getUpdateTiming() { return (float)gameLoop->getProfiler()->getReport("update");  }
		inline float getRenderTiming() { return (float)gameLoop->getProfiler()->getReport("render");  }
		inline float getSleepTiming() { return (float)gameLoop->getProfiler()->getReport("sleep");  }

	private:
		Application* application;
		GameLoop* gameLoop;
		DeferredRenderer* deferred_renderer;
		ScenesManager* scenes_manager;
	};

}
