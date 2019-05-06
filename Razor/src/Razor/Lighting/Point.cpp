#include "rzpch.h"
#include "Point.h"

namespace Razor {

	Point::Point() :
		Light(),
		position(glm::vec3(0.0f)),
		constant(1.0f),
		linear(0.09f),
		quadratic(0.032f)
	{
		type = Light::Type::POINT;
	}

	Point::~Point()
	{
	}

}
