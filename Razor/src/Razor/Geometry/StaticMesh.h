#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Material.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Maths/Maths.h"

namespace Razor {

	class RAZOR_API StaticMesh
	{
	public:
		StaticMesh();
		~StaticMesh();

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

		enum class CullType {
			FRONT = 0x0404,
			BACK = 0x0405
		};

		void draw();
		void setupBuffers();

		inline std::string& getName() { return this->name; }
		inline CullType getCullType() { return this->cullType; }
		inline bool hasCulling() { return this->culling; }
		inline Material* getMaterial() { return this->material; }
		inline std::vector<unsigned int>& getIndices() { return this->indices; }
		inline std::vector<float>& getVertices() { return this->vertices; }
		inline std::vector<float>& getUvs() { return this->uvs; }
		inline std::vector<float>& getNormals() { return this->normals; }
		inline std::vector<float>& getTangents() { return this->tangents; }
		inline unsigned int getVertexCount() { return vertexCount; }
		inline VertexArray* getVao() { return vao; }
		inline VertexBufferLayout* getVbl() { return vbl; }
		inline VertexBuffer* getVbo() { return vbo; }
		inline IndexBuffer* getIbo() { return ibo; }
		inline DrawMode getDrawMode() { return drawMode; }
		inline BoundingBox& getBoundingBox() { return bounding_box; }
		inline float getLineWidth() { return line_width; }
		inline bool isLineDashed() { return is_line_dashed; }
		inline int getLineFactor() { return line_factor; }
		inline unsigned short getLinePattern() { return line_pattern; }
		inline bool isReceivingShadows() { return receive_shadows; }

		inline void setName(const std::string& name) { this->name = name; }
		inline void setCullType(CullType type) { this->cullType = type; }
		inline void setHasCulling(bool value) { this->culling = value; }
		inline void setMaterial(Material* material) { this->material = material; }
		inline void setIndices(std::vector<unsigned int> indices) { this->indices = indices; }
		inline void setVertices(std::vector<float> vertices) { this->vertices = vertices; }
		inline void setUvs(std::vector<float> uvs) { this->uvs = uvs; }
		inline void setNormals(std::vector<float> normals) { this->normals = normals; }
		inline void setTangents(std::vector<float> tangents) { this->tangents = tangents; }
		inline void setVertexCount(unsigned int count) { this->vertexCount = count; }
		inline void setVao(VertexArray* vao) { this->vao = vao; }
		inline void setVbl(VertexBufferLayout* bvl) { this->vbl = vbl; }
		inline void setVbo(VertexBuffer* vbo) { this->vbo = vbo; }
		inline void setIbo(IndexBuffer* ibo) { this->ibo = ibo; }
		inline void setDrawMode(DrawMode mode) { drawMode = mode; }
		inline void setBoundingBox(const BoundingBox& box) { bounding_box = box; }
		inline void setLineWidth(float value) { line_width = value; }
		inline void setLineDashed(bool value) { is_line_dashed = value; }
		inline void setLineFactor(int factor) { line_factor = factor; }
		inline void setLinePattern(unsigned short pattern) { line_pattern = pattern; }
		inline void setIsReceivingShadows(bool value) { receive_shadows = value; }

	private:
		CullType cullType;
		bool culling;
		DrawMode drawMode;
		std::string name;
		Material* material;
		unsigned int vertexCount;

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<float> tangents;

		BoundingBox bounding_box;

		VertexArray* vao;
		VertexBufferLayout* vbl;
		VertexBuffer* vbo;
		IndexBuffer* ibo;

		float line_width;
		int line_factor;
		unsigned short line_pattern;
		bool is_line_dashed;
		bool receive_shadows;
	};

}

