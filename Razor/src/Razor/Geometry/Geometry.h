#pragma once

#include "StaticMesh.h"
#include "Razor/Maths/Maths.h"

namespace Razor 
{

	class Circle : public StaticMesh
	{
	public:
		Circle(float radius = 1.0f, unsigned int divisions = 32);
		~Circle();

	private:
		float radius;
		unsigned int divisions;
	};

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

	class Box : public StaticMesh
	{
	public:
		Box(float size = 1.0f);
		~Box();
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

	class Cone : public StaticMesh
	{
	public:
		Cone(unsigned int slices, float radius = 1.0f, float height = 2.0f);
		~Cone();

	private:
		unsigned int slices;
		float radius;
		float height;
	};

	class Cylinder : public StaticMesh
	{
	public:
		Cylinder();
		~Cylinder();
	};

	class Bounding : public StaticMesh
	{
	public:
		Bounding(const AABB& box);
		~Bounding();
	};

	class Grid : public StaticMesh
	{
	public:
		Grid(int divisions = 15);
		~Grid();

		inline void setDivisions(int divisions) { this->divisions = divisions; }
		inline int& getDivisions() { return this->divisions; }
		void generate();

	private:
		int divisions;
	};

	class Ray : public StaticMesh
	{
	public:
		Ray(
			const glm::vec3& origin = glm::vec3(0.0f),
			const glm::vec3& direction = glm::vec3(0.0f, -1.0, 0.0f),
			float length = 10.0f,
			bool normalized = false
		);
		~Ray();

	private:
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
		bool normalized;
	};

}
