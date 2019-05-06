#pragma once

#include "Razor/Geometry/StaticMesh.h"
#include <glm/glm.hpp>

namespace Razor
{

	class Terrain
	{
	public:
		Terrain(const glm::vec3& position);
		~Terrain();

		void generate();

		inline StaticMesh* getMesh() const { return mesh; }

	private:
		StaticMesh* mesh;
		glm::vec3 position;
		glm::vec2 size;
		glm::vec2 subdivisions;
	};

}
