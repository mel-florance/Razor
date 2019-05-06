#include "rzpch.h"
#include "Terrain.h"
#include <glm/gtx/string_cast.hpp>

namespace Razor
{

	Terrain::Terrain(const glm::vec3& position) : 
		position(position),
		mesh(nullptr),
		size(glm::vec2(100.0f, 100.0f)),
		subdivisions(glm::vec2(32.0f, 32.0f))
	{

	}

	Terrain::~Terrain()
	{
		if(mesh != nullptr)
			delete mesh;
	}

	void Terrain::generate()
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		mesh = new StaticMesh();
		mesh->setDrawMode(StaticMesh::DrawMode::TRIANGLE_STRIP);
		unsigned int row = 0, col = 0;

		for (row = 0; row <= subdivisions.y; row++)
		{
			for (col = 0; col <= subdivisions.x; col++)
			{
				glm::vec3 position = glm::vec3(
					(col * size.x) / subdivisions.x - (size.x / 2.0f),
					2.0f, 
					((subdivisions.y - row) * size.y) / subdivisions.y - (size.y / 2.0f)
				);

				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);

				vertices.push_back(0.0f);
				vertices.push_back(-1.0f);
				vertices.push_back(0.0f);

				vertices.push_back(col / subdivisions.x);
				vertices.push_back(1.0f - row / subdivisions.y);

				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);
			}
		}
		
		for (row = 0; row < subdivisions.y; row++)
		{
			for (col = 0; col < subdivisions.x; col++)
			{
				indices.push_back(col + 1 + (row + 1) * ((unsigned int)subdivisions.x + 1));
				indices.push_back(col + 1 + row * ((unsigned int)subdivisions.x + 1));
				indices.push_back(col + row * ((unsigned int)subdivisions.x + 1));
					
				indices.push_back(col + (row + 1) * ((unsigned int)subdivisions.x + 1));
				indices.push_back(col + 1 + (row + 1) * ((unsigned int)subdivisions.x + 1));
				indices.push_back(col + row * ((unsigned int)subdivisions.x + 1));
			}
		}
		
		mesh->setVertices(vertices);
		mesh->setIndices(indices);
		mesh->setupBuffers();
	}

}
