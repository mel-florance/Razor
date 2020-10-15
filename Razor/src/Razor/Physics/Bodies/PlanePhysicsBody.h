#pragma once

#include "Razor/Physics/PhysicsBody.h"

namespace Razor
{

	class PlanePhysicsBody : public PhysicsBody
	{
	public:
		PlanePhysicsBody(
			Node* node,
			const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f), 
			float constant = 0.0f
		);

		void init() override;

	private:
		glm::vec3 normal;
		float constant;
	};

}
