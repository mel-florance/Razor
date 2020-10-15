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
		isInstance(false),
		opened(false),
		active(true)
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

	void Node::setupMeshBuffers(std::shared_ptr<Node> node)
	{
		for (auto mesh : node->meshes)
			mesh->setupBuffers();

		for (auto n : node->nodes)
			setupMeshBuffers(n);
	}

}
