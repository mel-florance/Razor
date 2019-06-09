#pragma once

#include "Light.h"

namespace Razor {

	class Point : public Light
	{
	public:
		Point();
		~Point();

		inline glm::vec3& getPosition() { return position; }
		inline void setPosition(const glm::vec3& pos) { position = pos; }

		inline float& getConstant() { return constant; }
		inline void setConstant(float value) { constant = value; }

		inline float& getLinear() { return linear; }
		inline void setLinear(float value) { linear = value; }

		inline float& getQuadratic() { return quadratic; }
		inline void setQuadratic(float value) { quadratic = value; }

	private:
			glm::vec3 position;
			float constant;
			float linear;
			float quadratic;
	};

}
