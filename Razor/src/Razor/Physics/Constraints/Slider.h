#pragma once

#include "Razor/Physics/PhysicsConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"

namespace Razor
{

	class Slider : public PhysicsConstraint
	{
	public:
		Slider();
		~Slider();
	};

}
