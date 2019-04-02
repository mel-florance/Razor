#pragma once

#include "Razor/Core.h"
#include "Razor/Material.h"

namespace Razor {

	class RAZOR_API Mesh
	{
	public:
		Mesh();
		~Mesh();

		inline std::string& getName() { return this->name; }
		inline void setName(const std::string& name) { this->name = name; }

		inline std::vector<unsigned int>& getIndices() { return this->indices; }
		inline void setIndices(std::vector<unsigned int> indices) { this->indices = indices; }

		inline std::vector<float>& getVertices() { return this->vertices; }
		inline void setVertices(std::vector<float> vertices) { this->vertices = vertices; }

		inline std::vector<float>& getUvs() { return this->uvs; }
		inline void setUvs(std::vector<float> uvs) { this->uvs = uvs; }

		inline std::vector<float>& getNormals() { return this->normals; }
		inline void setNormals(std::vector<float> normals) { this->normals = normals; }

		inline std::vector<float>& getTangents() { return this->tangents; }
		inline void setTangents(std::vector<float> tangents) { this->tangents = tangents; }

		inline std::shared_ptr<Material> getMaterial() { return this->material; }
		inline void setMaterial(std::shared_ptr<Material> material) { this->material = material; }

	private:
		std::string name;
		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::shared_ptr<Material> material;
	};

}

