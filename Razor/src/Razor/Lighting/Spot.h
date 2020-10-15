#pragma once

#include "Light.h"

namespace Razor
{

	class Spot : public Light
	{
	public:
		Spot(
			Camera* camera,
			const glm::vec3& position = glm::vec3(0.0f, 1.0f, 0.0f),
			const glm::vec3& direction = glm::vec3(0.0f)
		);
		~Spot();

		inline glm::vec3& getPosition() { return position; }
		inline void setPosition(const glm::vec3& pos) { position = pos; }

		inline glm::vec3& getDirection() { return direction; }
		inline void setDirection(const glm::vec3& value) { direction = value; }

		inline float& getConstant() { return constant; }
		inline void setConstant(float value) { constant = value; }

		inline float& getLinear() { return linear; }
		inline void setLinear(float value) { linear = value; }

		inline float& getQuadratic() { return quadratic; }
		inline void setQuadratic(float value) { quadratic = value; }

		inline float& getInnerCutoff() { return inner_cutoff; }
		inline void setInnerCutoff(float value) { inner_cutoff = value; }

		inline float& getOuterCutoff() { return outer_cutoff; }
		inline void setOuterCutoff(float value) { outer_cutoff = value; }

	private:
		glm::vec3 position;
		glm::vec3 direction;
		float inner_cutoff;
		float outer_cutoff;
		float constant;
		float linear;
		float quadratic;
	};

}
