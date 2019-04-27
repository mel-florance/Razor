#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Core/Transform.h"
#include "Razor/Geometry/Mesh.h"

namespace Razor {

	class RAZOR_API Node
	{
	public:
		Node();
		Node(Node* n) {
			name = n->name;
			nodes = n->nodes;
			meshes = n->meshes;
			transform = n->transform;
			parent = n->parent;
			id = n->id;
		}
		~Node();

		std::string name;
		std::vector<Node*> nodes;
		std::vector<Mesh*> meshes;
		Transform transform;
		Node* parent;
		float distance; // for tests
		unsigned int id;
	};

}
