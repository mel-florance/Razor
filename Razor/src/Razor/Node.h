#pragma once

#include "Razor/Core.h"
#include "Razor/Mesh.h"

namespace Razor {

	class RZ_API Node
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
