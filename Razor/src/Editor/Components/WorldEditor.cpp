#include "rzpch.h"
#include "WorldEditor.h"
#include <imgui.h>

namespace Razor
{

	WorldEditor::WorldEditor(Editor* editor) : EditorComponent(editor)
	{
	}

	WorldEditor::~WorldEditor()
	{
	}

	void WorldEditor::render(float delta)
	{
		ImGui::Begin("World editor");

		ImGui::End();
	}
}
