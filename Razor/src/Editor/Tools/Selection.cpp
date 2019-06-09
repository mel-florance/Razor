#include "rzpch.h"
#include "Selection.h"
#include "Razor/Scene/Node.h"

namespace Razor
{

	Selection::Selection(Editor* editor) :
		EditorTool(editor),
		nodes({})
	{
	}

	Selection::~Selection()
	{
	}

	bool Selection::removeNode(unsigned int id)
	{
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
		[=](std::shared_ptr<Node> node) { return node->id == id;  }), nodes.end());

		return false;
	}

	bool Selection::isSelected(std::shared_ptr<Node> node)
	{
		auto it = std::find(nodes.begin(), nodes.end(), node);
		return it != nodes.end();
	}
}
