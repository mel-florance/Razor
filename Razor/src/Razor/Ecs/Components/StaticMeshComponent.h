#pragma once

#include "Razor/Ecs/Component.h"
#include "Razor/Geometry/StaticMesh.h"

namespace Razor {

	class StaticMeshComponent : public ECS::Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

		StaticMesh mesh;
	};

}
