#include "rzpch.h"
#include "SceneGraph.h"

namespace Razor {

	SceneGraph::SceneGraph() : nodes({})
	{
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::addNode(Node* node)
	{
		nodes.push_back(*node);
	}

}
