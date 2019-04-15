#pragma once

#include "Mesh.h"

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

}
