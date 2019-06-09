#include "rzpch.h"
#include "DeferredRenderer.h"
#include "Razor/Rendering/Pipelines/PbrPipeline.h"

namespace Razor
{

	DeferredRenderer::DeferredRenderer(Window* window, Engine* engine, ScenesManager* scenesManager, ShadersManager* shadersManager) :
		Renderer(),
		window(window),
		engine(engine),
		scenesManager(scenesManager),
		shadersManager(shadersManager)
	{
		pipelines["PBR"] = std::make_unique<PbrPipeline>();
	}

	DeferredRenderer::~DeferredRenderer()
	{
	}

}
