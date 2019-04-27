#pragma once

#include "Razor/Geometry/Mesh.h"
#include <glm/glm.hpp>

namespace Razor
{

	class Terrain
	{
	public:
		Terrain(const glm::vec3& position);
		~Terrain();

		void generate();

		inline Mesh* getMesh() const { return mesh; }

	private:
		Mesh* mesh;
		glm::vec3 position;
		glm::vec2 size;
		glm::vec2 subdivisions;
	};

}
