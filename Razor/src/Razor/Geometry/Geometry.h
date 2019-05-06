#pragma once

#include "StaticMesh.h"
#include "Razor/Maths/Maths.h"

namespace Razor {

	class Quad : public StaticMesh
	{
	public:
		Quad();
		~Quad();
	};

	class Plane : public StaticMesh
	{
	public:
		Plane(float radius = 1.0f);
		~Plane();

	private:
		float radius;
	};

	class Cube : public StaticMesh
	{
	public:
		Cube(float size = 1.0f);
		~Cube();
	};

	class UVSphere : public StaticMesh
	{
	public:
		UVSphere(float radius = 1.0f, const glm::ivec2& segments = glm::ivec2(16, 16));
		~UVSphere();

	private:
		float radius;
		glm::ivec2 segments;
	};

	class Bounding : public StaticMesh
	{
	public:
		Bounding(const BoundingBox& box);
		~Bounding();
	};

	class Grid : public StaticMesh
	{
	public:
		Grid(float divisions = 15.0f);
		~Grid();

		inline void setDivisions(float divisions) { this->divisions = divisions; }
		inline float& getDivisions() { return this->divisions; }
		void generate();

	private:
		float divisions;
	};

	class Ray : public StaticMesh
	{
	public:
		Ray(
			const glm::vec3& origin = glm::vec3(0.0f),
			const glm::vec3& direction = glm::vec3(0.0f, -1.0, 0.0f),
			float length = 10.0f
		);
		~Ray();

	private:
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
	};

}
