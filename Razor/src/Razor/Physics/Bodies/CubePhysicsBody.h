#pragma once

#include "Razor/Physics/PhysicsBody.h"

namespace Razor
{

	class CubePhysicsBody : public PhysicsBody
	{
	public:
		CubePhysicsBody(
			Node* node,
			const glm::vec3& extents = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& position = glm::vec3(0.0, 1.0f, 0.0f),
			const glm::vec3& rotation = glm::vec3(0.0, 1.0f, 0.0f)
			);
		~CubePhysicsBody();

		void init() override;

	private:
		glm::vec3 extents;
	};

}
