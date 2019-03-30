#pragma once

#include "Core.h"

namespace Razor {

	class RZ_API Application
	{
	public:
		Application();

		void run();

		virtual ~Application();
	};

	Application* createApplication();
}

