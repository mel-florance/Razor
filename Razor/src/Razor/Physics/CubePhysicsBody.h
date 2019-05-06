#pragma once

#include "PhysicsBody.h"

namespace Razor
{

	class CubePhysicsBody : public PhysicsBody
	{
	public:
		CubePhysicsBody(const glm::vec3& extents = glm::vec3(0.5f, 0.5f, 1.0f));
		~CubePhysicsBody();

		void init() override;

	private:
		glm::vec3 extents;
	};

}
