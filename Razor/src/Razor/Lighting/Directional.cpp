#include "rzpch.h"
#include "Directional.h"
#include "Razor/Cameras/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Razor
{

	Directional::Directional(Camera* camera, const glm::vec3& position, const glm::vec3& direction) :
		Light(camera),
		position(position),
		direction(direction)
	{
		type = Light::Type::DIRECTIONAL;
		light_bound = new DirectionalBound(this);
	}

	Directional::~Directional()
	{
	}

}
