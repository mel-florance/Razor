#pragma once

#include "PhysicsBody.h"

namespace Razor
{

	class SpherePhysicsBody : public PhysicsBody
	{
	public:
		SpherePhysicsBody(float radius = 1.0f, const glm::vec3& position = glm::vec3(0.0f));
		~SpherePhysicsBody();

		void init() override;

	private:
		float radius;
	};

}
