#include "rzpch.h"
#include "Landscape.h"
#include "Razor/Materials/Texture.h"
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor
{

	Landscape::Landscape(const std::string& filename, const glm::vec3& position) :
		filename(filename),
		position(position),
		mesh(nullptr),
		size(glm::vec2(100.0f, 100.0f)),
		subdivisions(glm::vec2(1024.0f, 1024.0f)),
		filter(glm::vec3(0.3f, 0.59f, 0.11f)),
		alpha_filter(0.0f),
		min_height(0.0f),
		max_height(0.05f),
		invert(false),
		components_count(4),
		heightmap_data(nullptr),
		heightmap_width(0),
		heightmap_height(0)
	{

	}

	Landscape::~Landscape()
	{
		
	}

	void Landscape::generate()
	{
		mesh = std::make_shared<StaticMesh>();
		mesh->setWindingOrder(StaticMesh::WindingOrder::CLOCKWISE);

		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::vector<float> uvs;
		std::vector<unsigned int> indices;

		heightmap_data = stbi_load_16(filename.c_str(), &heightmap_width, &heightmap_height, &components_count, STBI_rgb_alpha);

		if (min_height > max_height) {
			invert = true;
			float temp = max_height;
			max_height = min_height;
			min_height = temp;
		}

		for (unsigned int row = 0; row <= subdivisions.y; row++)
		{
			for (unsigned int col = 0; col <= subdivisions.x; col++)
			{
				glm::dvec3 position = glm::dvec3(
					(double)(col * size.x) / subdivisions.x - (size.x / 2.0f),
					0.0f, 
					(double)((subdivisions.y - row) * size.y) / subdivisions.y - (size.y / 2.0f)
				);

				glm::dvec2 heightmap_pos = glm::dvec2(
					(double)((position.x + size.x / 2.0f) / size.x) * (heightmap_width - 1.0f),
					(double)((1.0f - (position.z + size.y / 2.0f) / size.y) * (heightmap_height - 1.0f))
				);

				position.y = getHeightAtXZ(heightmap_pos);

				vertices.push_back((float)position.x);
				vertices.push_back((float)position.y);
				vertices.push_back((float)position.z);

				glm::dvec3 normal = calculateNormal(heightmap_pos);

				normals.push_back((float)normal.x);
				normals.push_back((float)normal.y);
				normals.push_back((float)normal.z);

				uvs.push_back((float)(col / subdivisions.x));
				uvs.push_back((float)(1.0f - row / subdivisions.y));

				tangents.push_back((float)position.x);
				tangents.push_back((float)position.y);
				tangents.push_back((float)position.z);
			}
		}
		
		for (unsigned int row = 0; row < subdivisions.y - 1; row++)
		{
			for (unsigned int col = 0; col < subdivisions.x - 1; col++)
			{
				unsigned int idx1 = (col + 1 + (row + 1) * ((unsigned int)subdivisions.x + 1));
				unsigned int idx2 = (col + 1 + row * ((unsigned int)subdivisions.x + 1));
				unsigned int idx3 = (col + row * ((unsigned int)subdivisions.x + 1));
				unsigned int idx4 = (col + (row + 1) * ((unsigned int)subdivisions.x + 1));

				bool isVisibleIdx1 = ((unsigned int)vertices[idx1 * 3 + 1] >= min_height);
				bool isVisibleIdx2 = ((unsigned int)vertices[idx2 * 3 + 1] >= min_height);
				bool isVisibleIdx3 = ((unsigned int)vertices[idx3 * 3 + 1] >= min_height);
				bool isVisibleIdx4 = ((unsigned int)vertices[idx4 * 3 + 1] >= min_height);

				if (isVisibleIdx4 && isVisibleIdx1 && isVisibleIdx3) 
				{
					indices.push_back(idx4);
					indices.push_back(idx1);
					indices.push_back(idx3);
				}

				if (isVisibleIdx1 && isVisibleIdx2 && isVisibleIdx3)
				{
					indices.push_back(idx1);
					indices.push_back(idx2);
					indices.push_back(idx3);
				}
			}
		}

		mesh->setVertexCount((unsigned int)subdivisions.y * (unsigned int)subdivisions.x * 3u);
		mesh->setVertices(vertices);
		mesh->setNormals(normals);
		mesh->setUvs(uvs);
		mesh->setTangents(tangents);
		mesh->setIndices(indices);

		mesh->setupBuffers();

		stbi_image_free(heightmap_data);
	}

	glm::vec3 Landscape::calculateNormal(const glm::vec2& position)
	{
		float height_l = getHeightAtXZ(glm::dvec2(position.x - 1, position.y));
		float height_r = getHeightAtXZ(glm::dvec2(position.x + 1, position.y));
		float height_d = getHeightAtXZ(glm::dvec2(position.x, position.y + 1));
		float height_u = getHeightAtXZ(glm::dvec2(position.x, position.y - 1));

		return glm::normalize(glm::dvec3(height_l - height_r, 0.1f, height_d - height_u));
	}

	float Landscape::getHeightAtXZ(const glm::vec2& position)
	{
		if (position.x < 0 || position.x >= heightmap_width || position.y < 0 || position.y >= heightmap_height)
			return 0.0f;

		float height = 0.0f;
		unsigned short* offset = heightmap_data + (unsigned int)(((double)position.x + (double)position.y * (double)heightmap_width) * 4);

		double r = offset[0] / 255.0;
		double g = offset[1] / 255.0;
		double b = offset[2] / 255.0;
		double a = offset[3] / 255.0;

		if (invert) {
			r = 1.0 - r;
			g = 1.0 - g;
			b = 1.0 - b;
		}

		float gradient = (float)(r * filter.x + g * filter.y + b * filter.z);

		if (a >= alpha_filter)
			height = min_height + (max_height - min_height) * gradient;
		else
			height = min_height;

		return height;
	}

}
