#include "rzpch.h"
#include "imgui.h"
#include "Tools.h"

namespace Razor {

	Tools::Tools(Editor* editor) : EditorComponent(editor)
	{
	}

	Tools::~Tools()
	{
	}

	void Tools::render()
	{
		ImGui::Begin("Tools");
		ImGui::End();
	}

}