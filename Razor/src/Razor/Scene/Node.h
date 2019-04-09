#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Geometry/Mesh.h"

namespace Razor {

	class RAZOR_API Node
	{
	public:
		Node();
		~Node();

		std::string name;
		std::vector<Node*> nodes;
		std::vector<Mesh*> meshes;
		Node* parent;
		unsigned int id;
	};

}
