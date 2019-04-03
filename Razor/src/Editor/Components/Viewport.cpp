#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"

namespace Razor {

	Viewport::Viewport(Editor* editor) : EditorComponent(editor)
	{
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::render()
	{
		ImGui::Begin("Viewport");
		
		ImGui::End();
	}

}