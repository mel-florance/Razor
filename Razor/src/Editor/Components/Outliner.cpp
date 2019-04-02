#include "rzpch.h"
#include "imgui.h"
#include "Outliner.h"

namespace Razor {

	Outliner::Outliner(Editor* editor) : EditorComponent(editor)
	{
	}

	Outliner::~Outliner()
	{
	}

	void Outliner::render()
	{
		ImGui::Begin("Outliner");
		ImGui::End();
	}

}