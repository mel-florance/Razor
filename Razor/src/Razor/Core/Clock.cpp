#include "rzpch.h"
#include "Clock.h"
#include "GLFW/glfw3.h"

namespace Razor {

	Clock::Clock()
	{

	}

	double Clock::getTime()
	{
		return time = glfwGetTime();
	}

	Clock::~Clock()
	{

	}

}
