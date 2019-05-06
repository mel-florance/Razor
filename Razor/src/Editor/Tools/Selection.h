#pragma once

#include "Editor/EditorTool.h"

namespace Razor
{
	class Node;

	class Selection : public EditorTool
	{
	public:
		Selection(Editor* editor);
		~Selection();

		inline void addNode(Node* node) { nodes.emplace_back(node); }
		inline std::vector<Node*>& getNodes() { return nodes; }

	private:
		std::vector<Node*> nodes;
	};

}
