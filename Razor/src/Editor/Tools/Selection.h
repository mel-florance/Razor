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

		inline void addNode(std::shared_ptr<Node> node) { clear();  nodes.push_back(node); }
		inline std::vector<std::shared_ptr<Node>>& getNodes() { return nodes; }
		bool removeNode(unsigned int id);
		bool isSelected(std::shared_ptr<Node> node);

		inline void clear() {
			nodes.clear();
		}

	private:
		std::vector<std::shared_ptr<Node>> nodes;
	};

}
