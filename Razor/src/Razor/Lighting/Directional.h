#pragma once

#include "Light.h"

namespace Razor {

	class Directional : public Light
	{
	public:
		Directional();
		~Directional();

	private:
		glm::vec3 direction;
	};

}
