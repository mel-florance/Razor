#include "rzpch.h"

#include "Application.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Log.h"

namespace Razor {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		WindowResizeEvent event(1280, 720);

		if (event.IsInCategory(EventCategoryApplication))
		{
			RZ_TRACE(event);
		}

		if (event.IsInCategory(EventCategoryInput))
		{
			RZ_TRACE(event);
		}

		while (true);
	}

}

