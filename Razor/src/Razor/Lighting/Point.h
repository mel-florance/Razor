#pragma once

#include "Light.h"

namespace Razor {

	class Point : public Light
	{
	public:
		Point();
		~Point();

	private:
		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;
	};

}
