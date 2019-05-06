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

		void drawNode(Node* node);
		void render(float delta) override;

	private:
		Selection* selection;
		ScenesManager* scenesManager;
	};

}
