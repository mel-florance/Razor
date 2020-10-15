#pragma once

#include "Light.h"

namespace Razor {

	class Directional : public Light
	{
	public:
		Directional(
			Camera* camera,
			const glm::vec3& position = glm::vec3(1.0f, 0.0f, 0.0f),
			const glm::vec3& direction = glm::vec3(0.0f)
		);
		~Directional();

		inline glm::vec3& getDirection() { return direction; }
		inline void setDirection(const glm::vec3& value) { direction = value; }

		inline glm::vec3& getPosition() { return position; }
		inline void setPosition(const glm::vec3& value) { position = value; }

	private:
		glm::vec3 direction;
		glm::vec3 position;
	};

}
