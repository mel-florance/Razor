#pragma once

#include "Node.h"

namespace Razor {

	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		inline std::vector<Node*>& getNodes() { return nodes; }
		inline void addNode(Node* node) { nodes.push_back(node); }

	private:
		std::vector<Node*> nodes;
	};

}
