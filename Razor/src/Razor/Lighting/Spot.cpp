#include "rzpch.h"
#include "Spot.h"

namespace Razor {

	Spot::Spot() :
		Light(),
		position(glm::vec3(0.0f)),
		direction(glm::vec3(0.0f)),
		inner_cutoff(glm::cos(glm::radians(12.0f))),
		outer_cutoff(glm::cos(glm::radians(15.0f))),
		constant(1.0f),
		linear(0.09f),
		quadratic(0.032f)
	{
		type = Light::Type::SPOT;
	}

	Spot::~Spot()
	{
	}

}
