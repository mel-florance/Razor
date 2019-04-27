#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Core/Utils.h"

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
			auto win_pos = ImGui::GetCursorPos();

			size = glm::vec2(win_size.x, win_size.y);
			ImGui::Image((void*)(intptr_t)tex, ImVec2(size.x, size.y - 22));

			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });

			ImGui::SetCursorPos(ImVec2(10.0f, 35.0f));
			ImGui::Text("FPS    %f", editor->getEngine()->getFPS());
			ImGui::SetCursorPos(ImVec2(10.0f, 55.0f));
			ImGui::Text("Frame  %f", editor->getEngine()->getFrameTiming());
			ImGui::SetCursorPos(ImVec2(10.0f, 75.0f));
			ImGui::Text("Update %f", editor->getEngine()->getUpdateTiming());
			ImGui::SetCursorPos(ImVec2(10.0f, 95.0f));
			ImGui::Text("Render %f", editor->getEngine()->getRenderTiming());
			ImGui::SetCursorPos(ImVec2(10.0f, 115.0f));
			ImGui::Text("Sleep  %f", editor->getEngine()->getSleepTiming());

			auto cam = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera();

			if (cam != nullptr)
			{
				float rangeA[] = { cam->getMinSpeed(), cam->getMaxSpeed() };
				float rangeB[] = { 1, 10 };
				float speed = Utils::mapValues(rangeA, rangeB, cam->getSpeed());

				char str[128];
				sprintf(str, "Camera speed %.1f", speed);
				ImVec2 margin = { 15.0f, 20.0f };
				ImVec2 text_size = ImGui::CalcTextSize(str);

				ImGui::SetCursorPos({
					size.x - text_size.x - margin.x,
					win_pos.x + text_size.y + margin.y 
				});

				ImGui::Text(str);
			}

			ImGui::PopStyleColor();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

}