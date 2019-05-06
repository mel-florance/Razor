#pragma once

#include "Light.h"

namespace Razor {

	class Directional : public Light
	{
	public:
		Directional();
		~Directional();

		inline glm::vec3 getDirection() { return direction; }
		inline void setDirection(const glm::vec3& value) { direction = value; }

	private:
		glm::vec3 direction;
	};

}
