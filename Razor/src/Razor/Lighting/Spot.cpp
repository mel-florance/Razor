#include "rzpch.h"
#include "Spot.h"
#include "Razor/Cameras/Camera.h"

namespace Razor 
{

	Spot::Spot(Camera* camera, const glm::vec3& position, const glm::vec3& direction) :
		Light(camera),
		position(position),
		direction(direction),
		inner_cutoff(glm::cos(glm::radians(12.0f))),
		outer_cutoff(glm::cos(glm::radians(15.0f))),
		constant(1.0f),
		linear(0.09f),
		quadratic(0.032f)
	{
		type = Light::Type::SPOT;
		light_bound = new SpotBound(this);
	}

	Spot::~Spot()
	{
	}

}
