#include "rzpch.h"
#include "Directional.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Razor {

	Directional::Directional() :
		Light(),
		direction(glm::vec3(0.0f))
	{
		type = Light::Type::DIRECTIONAL;
		shadow_generator.setViewMatrix(glm::lookAt(direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	Directional::~Directional()
	{
	}

}
