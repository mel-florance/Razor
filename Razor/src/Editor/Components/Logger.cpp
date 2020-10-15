#include "rzpch.h"
#include "Logger.h"
#include "Razor/Events/Event.h"
#include "Razor/Events/KeyEvent.h"
#include "Razor/Input/Input.h"
#include "Razor/Input/KeyCodes.h"
#include "Editor/Editor.h"

namespace Razor 
{

	Logger::Logger(Editor* editor) : 
		EditorComponent(editor),
		logs({}),
		hovered(false)
	{
		autoScroll = true;
		scrollToBottom = false;
		clear();
	}

	Logger::~Logger()
	{
	}

	void Logger::addLog(const std::string& str)
	{
		logs.push_back(str);

		if (autoScroll)
			scrollToBottom = true;
	}

	void Logger::clear()
	{
		logs.clear();
	}

	void Logger::render(float delta)
	{
		ImGui::Begin("Log");

		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		float height = ImGui::GetWindowHeight();
		hovered = ImGui::IsWindowHovered();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		
		ImGui::Columns(2, "position_columns", true);
		ImGui::SetColumnWidth(0, 30.0f);

		ImGui::SetCursorScreenPos(ImGui::GetWindowPos());
		editor->icons_manager->drawIcon("clear", glm::vec2(26.0f), IconsManager::IconType::BUTTON);
		
		if (ImGui::IsItemClicked())
			clear();

		ImGui::Dummy(ImVec2(30.0f, 4.0f));
		
		editor->icons_manager->drawIcon("copy", glm::vec2(26.0f), IconsManager::IconType::BUTTON);
		ImGui::Dummy(ImVec2(30.0f, 4.0f));
		editor->icons_manager->drawIcon("export", glm::vec2(26.0f), IconsManager::IconType::BUTTON);
		ImGui::Dummy(ImVec2(30.0f, height - (23.0f * 3) - 8.0f));
		ImGui::NextColumn();
		ImGui::Indent(5.0f);

		for (auto log : logs)
			ImGui::Text(log.c_str());

		ImGui::Indent(-5.0f);
		ImGui::PopStyleVar();

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);

		scrollToBottom = false;
		ImGui::EndChild();

		ImGui::End();
	}

	void Logger::onEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_L &&
				Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL) && 
				hovered)
			{
				clear();
			}
		}
	}

}