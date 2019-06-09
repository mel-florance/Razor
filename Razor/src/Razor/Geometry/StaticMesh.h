#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Material.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Maths/Maths.h"

namespace Razor 
{
	class PhysicsBody;
	class Transform;

	class StaticMesh
	{
	public:
		StaticMesh();
		virtual ~StaticMesh();

		enum class DrawMode
		{
			POINTS         = 0x0000,
			LINES          = 0x0001,
			LINE_LOOP      = 0x0002,
			LINE_STRIP     = 0x0003,
			TRIANGLES      = 0x0004,
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN   = 0x0006,
			QUADS          = 0x0007
		};

		static const std::array<const char*, 8> getDrawModesStr()
		{
			return {
				"Points",
				"Lines",
				"Line loop",
				"Line strip",
				"Triangles",
				"Triangle strip",
				"Triangle fan",
				"Quads"
			};
		}

		inline const char* getDrawModeStr() 
		{
			switch (drawMode) {
			default:
			case DrawMode::TRIANGLES:      return "Triangles"; break;
			case DrawMode::POINTS:         return "Points"; ; break;
			case DrawMode::LINES:          return "Lines"; ; break;
			case DrawMode::LINE_LOOP:      return "Line loop" ; break;
			case DrawMode::LINE_STRIP:     return "Line strip" ; break;
			case DrawMode::TRIANGLE_STRIP: return "Triangle strip"; break;
			case DrawMode::TRIANGLE_FAN:   return "Triangle fan"; break;
			case DrawMode::QUADS:          return "Quads" ; break;
			}
		}

		inline void setDrawMode(const char* mode) 
		{
			if (mode == "Points")         drawMode = DrawMode::POINTS;
			if (mode == "Lines")          drawMode = DrawMode::LINES;
			if (mode == "Line loop")      drawMode = DrawMode::LINE_LOOP;
			if (mode == "Line strip")     drawMode = DrawMode::LINE_STRIP;
			if (mode == "Triangles")      drawMode = DrawMode::TRIANGLES;
			if (mode == "Triangle strip") drawMode = DrawMode::TRIANGLE_STRIP;
			if (mode == "Triangle fan")   drawMode = DrawMode::TRIANGLE_FAN;
			if (mode == "Quads")          drawMode = DrawMode::QUADS;
		}

		enum class CullType
		{
			FRONT_LEFT     = 0x0400,
			FRONT_RIGHT    = 0x0401,
			BACK_LEFT      = 0x0402,
			BACK_RIGHT     = 0x0403,
			FRONT          = 0x0404,
			BACK           = 0x0405,
			LEFT           = 0x0406,
			RIGHT          = 0x0407,
			FRONT_AND_BACK = 0x0408
		};
		
		static const std::array<const char*, 9> getCullTypesStr() 
		{
			return { 
				"Front left",
				"Front right",
				"Back left",
				"Back right",
				"Front", 
				"Back" ,
				"Left",
				"Right",
				"Front & back"
			};
		}

		inline const char* getCullTypeStr() 
		{
			switch (cullType) {
			default:
			case CullType::BACK:            return "Back"; break;
			case CullType::FRONT_LEFT:     return "Front left"; break;
			case CullType::FRONT_RIGHT:     return "Front right"; break;
			case CullType::BACK_LEFT:       return "Back left"; break;
			case CullType::BACK_RIGHT:      return "Front right"; break;
			case CullType::FRONT:           return "Front"; break;
			case CullType::LEFT:            return "Left"; break;
			case CullType::RIGHT:           return "Right"; break;
			case CullType::FRONT_AND_BACK:  return "Front & back"; break;
			}
		}

		inline void setCullType(const char* mode)
		{
			if (mode == "Front left")    cullType = CullType::FRONT_LEFT;
			if (mode == "Front right")   cullType = CullType::FRONT_RIGHT;
			if (mode == "Back left")     cullType = CullType::BACK_LEFT;
			if (mode == "Back right")    cullType = CullType::BACK_RIGHT;
			if (mode == "Front")         cullType = CullType::FRONT;
			if (mode == "Back")          cullType = CullType::BACK;
			if (mode == "Left")          cullType = CullType::LEFT;
			if (mode == "Right")         cullType = CullType::RIGHT;
			if (mode == "Front & back")  cullType = CullType::FRONT_AND_BACK;
		}


		enum class WindingOrder
		{
			CLOCKWISE = 0x0900,
			COUNTER_CLOCKWISE = 0x0901
		};

		static const std::array<const char*, 2> getWindingOrdersStr() 
		{
			return {
				"Clockwise",
				"Counter-Clockwise"
			};
		}

		inline const char* getWindingOrderStr() 
		{
			switch (windingOrder) {
			default:
			case WindingOrder::COUNTER_CLOCKWISE: return "Counter-Clockwise"; break;
			case WindingOrder::CLOCKWISE:         return "Clockwise"; break;
			}
		}

		inline void setWindingOrder(const char* order) 
		{
			if (order == "Clockwise")    windingOrder = WindingOrder::CLOCKWISE;
			if (order == "Counter-Clockwise")   windingOrder = WindingOrder::COUNTER_CLOCKWISE;
		}

		void draw();
		void setupBuffers();
		void setupInstances(std::vector<glm::mat4>& matrices);
		void updateInstance(const glm::mat4& matrix, unsigned int index);
		void calculateTangents();

		inline std::string& getName() { return this->name; }
		inline CullType getCullType() { return this->cullType; }
		inline WindingOrder getWindingOrder() { return this->windingOrder; }
		inline bool& hasCulling() { return this->culling; }
		inline std::shared_ptr<Material> getMaterial() { return this->material; }
		inline std::vector<unsigned int>& getIndices() { return this->indices; }
		inline std::vector<float>& getVertices() { return this->vertices; }
		inline std::vector<float>& getUvs() { return this->uvs; }
		inline std::vector<float>& getNormals() { return this->normals; }
		inline std::vector<float>& getTangents() { return this->tangents; }
		inline unsigned int getVertexCount() { return vertexCount; }
		inline VertexArray* getVao() { return vao; }
		inline VertexBuffer* getVbo() { return vbo; }
		inline IndexBuffer* getIbo() { return ibo; }
		inline DrawMode getDrawMode() { return drawMode; }
		inline BoundingBox& getBoundingBox() { return bounding_box; }
		inline float& getLineWidth() { return line_width; }
		inline bool& isLineDashed() { return is_line_dashed; }
		inline int& getLineFactor() { return line_factor; }
		inline int& getLinePattern() { return line_pattern; }
		inline bool& isReceivingShadows() { return receive_shadows; }
		inline bool& isBoundingBoxVisible() { return show_bounding_box; }
		inline PhysicsBody* getPhysicsBody() { return body;  }
		inline bool& getPhysicsEnabled() { return physics_enabled; }

		inline void setName(const std::string& name) { this->name = name; }
		inline void setCullType(CullType type) { this->cullType = type; }
		inline void setHasCulling(bool value) { this->culling = value; }
		inline void setMaterial(std::shared_ptr<Material> material) { this->material = material; }
		inline void setIndices(std::vector<unsigned int> indices) { this->indices = indices; }
		inline void setVertices(std::vector<float> vertices) { this->vertices = vertices; }
		inline void setUvs(std::vector<float> uvs) { this->uvs = uvs; }
		inline void setNormals(std::vector<float> normals) { this->normals = normals; }
		inline void setTangents(std::vector<float> tangents) { this->tangents = tangents; }
		inline void setVertexCount(unsigned int count) { this->vertexCount = count; }
		inline void setVao(VertexArray* vao) { this->vao = vao; }
		inline void setVbo(VertexBuffer* vbo) { this->vbo = vbo; }
		inline void setIbo(IndexBuffer* ibo) { this->ibo = ibo; }
		inline void setDrawMode(DrawMode mode) { drawMode = mode; }
		inline void setWindingOrder(WindingOrder order) { windingOrder = order; }
		inline void setBoundingBox(const BoundingBox& box) { bounding_box = box; }
		inline void setLineWidth(float value) { line_width = value; }
		inline void setLineDashed(bool value) { is_line_dashed = value; }
		inline void setLineFactor(int factor) { line_factor = factor; }
		inline void setLinePattern(int pattern) { line_pattern = pattern; }
		inline void setIsReceivingShadows(bool value) { receive_shadows = value; }
		inline void setBoundingBoxVisible(bool value) { show_bounding_box = value; }
		inline void setPhysicsBody(PhysicsBody* body) { this->body = body; }
		inline void setPhysicsEnabled(bool value) { physics_enabled = value; }

	protected:
		VertexArray* vao;
		VertexBuffer* vbo;
		VertexBuffer* nbo;
		VertexBuffer* tbo;
		VertexBuffer* uvbo;
		IndexBuffer* ibo;
		unsigned int mbo;

	private:
		std::string name;
		std::shared_ptr<Material> material;

		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> bonesIds;
		std::vector<float> bonesWeights;

		bool culling;
		WindingOrder windingOrder;
		CullType cullType;
		DrawMode drawMode; 

		bool receive_shadows;
		unsigned int vertexCount;

		bool is_line_dashed;
		float line_width;
		int line_factor;
		int line_pattern;

		BoundingBox bounding_box;
		bool show_bounding_box;

		PhysicsBody* body;
		bool physics_enabled;
	};

}

