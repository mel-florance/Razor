#pragma once

#include "Razor/Physics/PhysicsConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"

namespace Razor
{

	class Hinge : public PhysicsConstraint
	{
	public:
		Hinge();
		~Hinge();
	};

}
