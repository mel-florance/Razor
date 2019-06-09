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

		inline std::shared_ptr<Node> createInstance(std::shared_ptr<StaticMesh> mesh)
		{
			std::shared_ptr<Node> node = std::make_shared<Node>();
			instances[mesh].push_back(node);
		}

	private:
		std::vector<std::shared_ptr<Node>> nodes;
		std::map<std::shared_ptr<StaticMesh>, std::vector<std::shared_ptr<Node>>> instances;
		unsigned int index;
	};

}
