#pragma once

#include "Razor/Geometry/StaticMesh.h"
#include <glm/glm.hpp>

namespace Razor
{
	class Texture;

	class Landscape
	{
	public:
		Landscape(const std::string& filename, const glm::dvec3& position = glm::dvec3(0.0f, 0.0f, 0.0f));
		~Landscape();

		void generate();

		inline std::shared_ptr<StaticMesh> getMesh() const { return mesh; }
		glm::dvec3 calculateNormal(const glm::dvec2& position);
		float getHeightAtXZ(const glm::dvec2& position);

	private:
		std::string filename;
		std::shared_ptr<StaticMesh> mesh;
		glm::dvec3 position;
		glm::dvec2 size;
		glm::dvec2 subdivisions;

		glm::dvec3 filter;
		float alpha_filter;
		float min_height;
		float max_height;
		bool invert = false;

		int heightmap_width;
		int heightmap_height;
		int components_count;

		unsigned short* heightmap_data;
	};

}
