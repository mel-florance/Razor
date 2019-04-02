#include "rzpch.h"
#include "Node.h"

namespace Razor {

	Node::Node() :
		name(),
		nodes({}),
		meshes({}),
		parent(),
		id(0)
	{
	}

	Node::~Node()
	{
	}

}
