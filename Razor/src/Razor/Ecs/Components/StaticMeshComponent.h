#pragma once

#include "Razor/Ecs/Component.h"
#include "Razor/Geometry/Mesh.h"

namespace Razor {

	class StaticMeshComponent : public ECS::Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

		Mesh mesh;
	};

}
