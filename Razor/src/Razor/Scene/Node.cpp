#include "rzpch.h"
#include "Node.h"
#include "Razor/Lighting/Light.h"

namespace Razor
{

	Node::Node() :
		name(""),
		nodes({}),
		meshes({}),
		lights({}),
		parent(nullptr),
		id(0),
		isInstance(false)
	{
	}

	Node::~Node()
	{
		for (auto light : lights)
			light.reset();

		for (auto mesh : meshes)
			mesh.reset();

		for (auto node : nodes) {
			node->parent.reset();
			node.reset();
		}

		parent.reset();
		meshes.clear();
		nodes.clear();
		lights.clear();
	}

}
