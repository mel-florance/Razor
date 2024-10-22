#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class ScenesManager;
	class Node;
	class Selection;

	class RAZOR_API Outliner : public EditorComponent
	{
	public:
		Outliner(Editor* editor);
		~Outliner();

		void drawNode(std::shared_ptr<Node> node, unsigned int index, unsigned int depth);
		void render(float delta) override;
		void onEvent(Event& event) override;

	private:
		Selection* selection;
		ScenesManager* scenesManager;
		std::string filtered_name;
		std::shared_ptr<Node> focused_node;
	};

}
