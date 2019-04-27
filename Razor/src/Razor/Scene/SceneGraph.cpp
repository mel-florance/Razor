#include "rzpch.h"
#include "SceneGraph.h"

namespace Razor {

	SceneGraph::SceneGraph() : nodes({}), index(0)
	{
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::addNode(Node* node)
	{
		node->id = ++index;
		nodes.push_back(*node);
	}

}
