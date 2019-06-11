#pragma once

#include "Node.h"

namespace Razor 
{

	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		inline std::vector<std::shared_ptr<Node>>& getNodes() { return nodes; }
		void addNode(std::shared_ptr<Node> node);

		bool removeNode(unsigned int id);

	private:
		std::vector<std::shared_ptr<Node>> nodes;
		unsigned int index;
	};

}
