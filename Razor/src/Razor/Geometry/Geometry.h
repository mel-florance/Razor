#pragma once

#include "Mesh.h"
#include "Razor/Maths/Maths.h"

namespace Razor {

	class Quad : public Mesh
	{
	public:
		Quad();
		~Quad();
	};

	class Plane : public Mesh
	{
	public:
		Plane();
		~Plane();
	};

	class Cube : public Mesh
	{
	public:
		Cube();
		~Cube();
	};

	class Sphere : public Mesh
	{
	public:
		Sphere();
		~Sphere();
	};

	class Bounding : public Mesh
	{
	public:
		Bounding(const BoundingBox& box);
		~Bounding();
	};

}
