#include "rzpch.h"
#include "Viewport.h"

namespace Razor
{

	Viewport::Viewport(Window* window, const glm::vec2& position, const glm::vec2& size) :
		window(window),
		position(position),
		size(size),
		hovered(false),
		visible(true)
	{

	}

	Viewport::~Viewport()
	{

	}

}
