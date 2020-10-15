#pragma once

#include "Razor/Geometry/StaticMesh.h"
#include <glm/glm.hpp>

namespace Razor
{
	class Texture;

	class Landscape
	{
	public:
		typedef std::vector<std::shared_ptr<Landscape>> List;

		Landscape(const std::string& filename, const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
		~Landscape();

		void generate();

		inline std::shared_ptr<StaticMesh> getMesh() const { return mesh; }
		glm::vec3 calculateNormal(const glm::vec2& position);
		float getHeightAtXZ(const glm::vec2& position);

	private:
		std::string filename;
		std::shared_ptr<StaticMesh> mesh;
		glm::vec3 position;
		glm::vec2 size;
		glm::vec2 subdivisions;

		glm::vec3 filter;
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
