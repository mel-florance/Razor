#include "rzpch.h"
#include "Logger.h"

namespace Razor {

	Logger::Logger(Editor* editor) : 
		EditorComponent(editor),
		logs({})
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
		logs.emplace_back(str);

		if (autoScroll)
			scrollToBottom = true;
	}

	void Logger::clear()
	{
		//logs.clear();
	}

	void Logger::render(float delta)
	{
		ImGui::Begin("Log");

		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		
		for (auto log : logs)
			ImGui::Text(log.c_str());

		ImGui::PopStyleVar();

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);

		scrollToBottom = false;
		ImGui::EndChild();

		ImGui::End();
	}

}