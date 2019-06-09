#pragma once

#include "Renderer.h"

namespace Razor
{

	class Window;
	class Engine;
	class ScenesManager;
	class ShadersManager;

	class DeferredRenderer : public Renderer
	{
	public:
		DeferredRenderer(Window* window, Engine* engine, ScenesManager* scenesManager, ShadersManager* shadersManager);
		~DeferredRenderer();

	private:
		Window* window;
		Engine* engine;
		ScenesManager* scenesManager;
		ShadersManager* shadersManager;
	};

}
