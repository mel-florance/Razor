#include "rzpch.h"
#include "SceneGraph.h"

namespace Razor {

	SceneGraph::SceneGraph() : nodes({}), index(0)
	{
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::addNode(std::shared_ptr<Node> node)
	{
		node->id = index++;
		nodes.push_back(node);
	}

	bool SceneGraph::removeNode(unsigned int id)
	{
		std::vector<std::shared_ptr<Node>>::iterator it = std::find_if(nodes.begin(), nodes.end(), [=](const std::shared_ptr<Node> n) {
			return n->id == id;
		});

		if (it != nodes.end())
		{
			it->reset();
			nodes.erase(it);

			return true;
		}

		return false;
	}

	std::shared_ptr<Node> SceneGraph::getNodeById(unsigned int id)
	{
		std::vector<std::shared_ptr<Node>>::iterator it;

		for (it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it)->id == id)
				return *it;
		}

		return nullptr;
	}

}
