#pragma once

#include "Core.h"

namespace Razor {

	class RZ_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};

	Application* createApplication();
}

