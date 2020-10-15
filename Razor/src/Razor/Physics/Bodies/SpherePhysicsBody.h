#pragma once

#include "Razor/Physics/PhysicsBody.h"

namespace Razor
{

	class SpherePhysicsBody : public PhysicsBody
	{
	public:
		SpherePhysicsBody(
			Node* node,
			float radius = 1.0f
			);

		void init() override;

	private:
		float radius;
	};

}
