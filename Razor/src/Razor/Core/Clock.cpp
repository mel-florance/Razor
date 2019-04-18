#include "rzpch.h"
#include "Clock.h"

namespace Razor {

	Clock::Clock() : m_epoch(std::chrono::high_resolution_clock::now())
	{

	}

	double Clock::getTime()
	{
		return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::high_resolution_clock::now() - m_epoch).count();
	}

	Clock::~Clock()
	{

	}

}
