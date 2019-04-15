#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"

namespace Razor {

	Viewport::Viewport(Editor* editor) : EditorComponent(editor), hovered(false)
	{
		
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::render(float delta)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		DeferredRenderer* renderer = Application::Get().getDeferredRenderer();

		if (renderer != nullptr)
		{
			auto buffer = renderer->getColorBuffer();
			auto tex = buffer->getId();
			auto win_size = ImGui::GetWindowSize();
			size = glm::vec2(win_size.x, win_size.y);
			ImGui::Image((void*)(intptr_t)tex, ImVec2(size.x, size.y - 22));

			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });
			ImGui::SetCursorPos(ImVec2(10.0f, 35.0f));
			ImGui::Text("FPS %f", editor->getEngine()->getFPS());
			ImGui::PopStyleColor();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

}