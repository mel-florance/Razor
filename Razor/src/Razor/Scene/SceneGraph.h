#pragma once

#include "Node.h"

namespace Razor {

	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		inline std::vector<Node*>& getNodes() { return nodes; }

		void addNode(Node* node);

	private:
		std::vector<Node*> nodes;
		unsigned int index;
	};

}
