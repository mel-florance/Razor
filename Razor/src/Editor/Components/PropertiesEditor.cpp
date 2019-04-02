#include "rzpch.h"
#include "imgui.h"
#include "PropertiesEditor.h"

namespace Razor {

	PropertiesEditor::PropertiesEditor(Editor* editor) : EditorComponent(editor)
	{
	}

	PropertiesEditor::~PropertiesEditor()
	{
	}

	void PropertiesEditor::render()
	{
		ImGui::Begin("Properties Editor");
		ImGui::End();
	}

}