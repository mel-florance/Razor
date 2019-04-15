#include "rzpch.h"
#include "Logger.h"

namespace Razor {

	Logger::Logger(Editor* editor) : EditorComponent(editor)
	{
		autoScroll = true;
		scrollToBottom = false;
		clear();
	}

	Logger::~Logger()
	{
	}

	void Logger::addLog(const char* fmt, ...)
	{
		int old_size = buffer.size();
		va_list args;
		va_start(args, fmt);
		buffer.appendfv(fmt, args);
		va_end(args);

		for (int new_size = buffer.size(); old_size < new_size; old_size++)
			if (buffer[old_size] == '\n')
				lineOffsets.push_back(old_size + 1);

		if (autoScroll)
			scrollToBottom = true;
	}

	void Logger::clear()
	{
		buffer.clear();
		lineOffsets.clear();
		lineOffsets.push_back(0);
	}

	void Logger::render(float delta)
	{
		ImGui::Begin("Log");

		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = buffer.begin();
		const char* buf_end = buffer.end();

		if (filter.IsActive())
		{
			for (int line_no = 0; line_no < lineOffsets.Size; line_no++)
			{
				const char* line_start = buf + lineOffsets[line_no];
				const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;

				if (filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(lineOffsets.Size);

			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + lineOffsets[line_no];
					const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}

			clipper.End();
		}

		ImGui::PopStyleVar();

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);

		scrollToBottom = false;
		ImGui::EndChild();

		ImGui::End();
	}

}