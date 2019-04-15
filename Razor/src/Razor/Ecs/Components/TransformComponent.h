#pragma once

#include "Razor/Ecs/Component.h"
#include "Razor/Core/Transform.h"

namespace Razor {

	class TransformComponent : public ECS::Component
	{
	public:
		TransformComponent();
		~TransformComponent();

		Transform transform;
	};

}



