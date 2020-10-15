#include "rzpch.h"
#include "Point.h"
#include "Razor/Cameras/Camera.h"

namespace Razor {

	Point::Point(Camera* camera, const glm::vec3& position) :
		Light(camera),
		position(position),
		constant(1.0f),
		linear(0.7f),
		quadratic(1.8f)
	{
		type = Light::Type::POINT;
		light_bound = new PointBound(this);
	}

	Point::~Point()
	{
	}

}
