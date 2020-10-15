#pragma once

#include "Node.h"

namespace Razor 
{

	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		typedef std::vector<std::shared_ptr<Node>> NodeList;

		inline NodeList& getNodes() { return nodes; }
		void addNode(std::shared_ptr<Node> node);

		bool removeNode(unsigned int id);

		std::shared_ptr<Node> getNodeById(unsigned int id);

	private:
		NodeList nodes;
		unsigned int index;
	};

}
