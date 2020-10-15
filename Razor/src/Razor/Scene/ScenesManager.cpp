#include "rzpch.h"
#include "ScenesManager.h"

namespace Razor
{

	ScenesManager::ScenesManager()
	{
		std::shared_ptr<Razor::Scene> scene = std::make_shared<Razor::Scene>("default_scene");
		setActiveScene(scene);
	}

	ScenesManager::~ScenesManager()
	{
	}

}
