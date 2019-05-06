#include "rzpch.h"
#include "Directional.h"

namespace Razor {

	Directional::Directional() :
		Light(),
		direction(glm::vec3(0.0f))
	{
		type = Light::Type::DIRECTIONAL;
	}

	Directional::~Directional()
	{
	}

}
