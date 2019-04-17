#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Material.h"
#include "Razor/Buffers/Buffers.h"

namespace Razor {

	class RAZOR_API Mesh
	{
	public:
		Mesh();
		~Mesh();

		enum class DrawMode {
			POINTS = 0x0000,
			LINES = 0x0001,
			LINE_LOOP = 0x0002,
			LINE_STRIP = 0x0003,
			TRIANGLES = 0x0004,
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN = 0x0006,
			QUADS = 0x0007
		};

		void draw();
		void setupBuffers();

		inline std::string& getName() { return this->name; }
		inline void setName(const std::string& name) { this->name = name; }

		inline std::shared_ptr<Material> getMaterial() { return this->material; }
		inline void setMaterial(std::shared_ptr<Material> material) { this->material = material; }

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

		inline unsigned int getVertexCount() { return vertexCount; }
		inline void setVertexCount(unsigned int count) { this->vertexCount = count; }

		inline VertexArray* getVao() { return vao; }
		inline void setVao(VertexArray* vao) { this->vao = vao; }

		inline VertexBufferLayout* getVbl() { return vbl; }
		inline void setVbl(VertexBufferLayout* bvl) { this->vbl = vbl; }

		inline VertexBuffer* getVbo() { return vbo; }
		inline void setVbo(VertexBuffer* vbo) { this->vbo = vbo; }

		inline IndexBuffer* getIbo() { return ibo; }
		inline void setIbo(IndexBuffer* ibo) { this->ibo = ibo; }

		inline DrawMode getDrawMode() { return drawMode; }
		inline void setDrawMode(DrawMode mode) { drawMode = mode; }

	private:
		DrawMode drawMode;
		std::string name;
		std::shared_ptr<Material> material;
		unsigned int vertexCount;

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<float> tangents;

		VertexArray* vao;
		VertexBufferLayout* vbl;
		VertexBuffer* vbo;
		IndexBuffer* ibo;
	};

}

