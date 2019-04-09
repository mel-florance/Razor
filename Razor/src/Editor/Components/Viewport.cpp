#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"
#include "Razor/Application/Application.h"

namespace Razor {

	Viewport::Viewport(Editor* editor) : EditorComponent(editor)
	{
		
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::render()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");

		auto tex = Application::Get().getForwardRenderer()->getColorBuffer()->getId();
		size = ImGui::GetWindowSize();
		ImGui::Image((void*)(intptr_t)tex, ImVec2(size.x, size.y - 22));

		ImGui::End();

		ImGui::PopStyleVar();
	}

}