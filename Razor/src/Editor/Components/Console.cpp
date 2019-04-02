#include "rzpch.h"
#include "imgui.h"
#include "Console.h"

namespace Razor {

	Console::Console(Editor* editor) : EditorComponent(editor)
	{
		clearLog();
		memset(input, 0, sizeof(input));
		historyPos = -1;
		commands.push_back("HELP");
		commands.push_back("HISTORY");
		commands.push_back("CLEAR");
		autoScroll = true;
		scrollToBottom = true;
		addLog("Welcome to the razor engine!");
	}

	Console::~Console()	
	{
		clearLog();
		for (int i = 0; i < history.size(); i++)
			free(history[i]);
	}

	void Console::render()
	{
		ImGui::Begin("Console");

		bool copy_to_clipboard = false;

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		if (copy_to_clipboard)
			ImGui::LogToClipboard();

		for (int i = 0; i < items.size(); i++)
		{
			const char* item = items[i];

			if (!filter.PassFilter(item))
				continue;

			bool pop_color = false;

			if (strstr(item, "[error]")) { 
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); 
				pop_color = true; 
			}
			else if (strncmp(item, "# ", 2) == 0) { 
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
				pop_color = true; 
			}

			ImGui::TextUnformatted(item);

			if (pop_color)
				ImGui::PopStyleColor();
		}

		if (copy_to_clipboard)
			ImGui::LogFinish();

		if (scrollToBottom)
			ImGui::SetScrollHereY(1.0f);

		scrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		//ImGui::Separator();

		bool reclaim_focus = false;

		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##Command",
			input,
			IM_ARRAYSIZE(input), 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory,
			&TextEditCallbackStub, 
			(void*)this))	
		{
			char* s = input;
			Strtrim(s);

			if (s[0])
				execCommand(s);

			strcpy(s, "");
			reclaim_focus = true;
		}
		ImGui::PopItemWidth();

		ImGui::SetItemDefaultFocus();

		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}

	void Console::clearLog()
	{
		for (int i = 0; i < items.size(); i++)
			free(items[i]);

		items.clear();
		scrollToBottom = true;
	}

	void Console::addLog(const char* fmt, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		items.push_back(Strdup(buf));
		if (autoScroll)
			scrollToBottom = true;
	}

	void Console::execCommand(const char* command_line)
	{
		addLog("# %s\n", command_line);

		historyPos = -1;

		for (int i = (int)history.size() - 1; i >= 0; i--) {
			if (Stricmp(history[i], command_line) == 0)
			{
				free(history[i]);
				history.erase(history.begin() + i);
				break;
			}
		}
	
		history.push_back(Strdup(command_line));


		if (Stricmp(command_line, "CLEAR") == 0)
		{
			clearLog();
		}
		else if (Stricmp(command_line, "HELP") == 0)
		{
			addLog("Commands:");
			for (int i = 0; i < commands.size(); i++)
				addLog("- %s", commands[i]);
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = (int)history.size() - 10;
			for (int i = first > 0 ? first : 0; i < history.size(); i++)
				addLog("%3d: %s\n", i, history[i]);
		}
		else
		{
			addLog("Unknown command: '%s'\n", command_line);
		}

		// On commad input, we scroll to bottom even if AutoScroll==false
		scrollToBottom = true;
	}

	int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		switch (data->EventFlag)
		{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				const char* word_end = data->Buf + data->CursorPos;
				const char* word_start = word_end;

				while (word_start > data->Buf)
				{
					const char c = word_start[-1];

					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;

					word_start--;
				}

				std::vector<const char*> candidates;

				for (int i = 0; i < commands.size(); i++)
					if (Strnicmp(commands[i], word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(commands[i]);

				if (candidates.size() == 0)
				{
					std::stringstream a;
					a << ((int)(word_end - word_start));
					a << ".";
					a << word_start;
					addLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				}
				else if (candidates.size() == 1)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;

						for (int i = 0; i < candidates.size() && all_candidates_matches; i++) {
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						}

						if (!all_candidates_matches)
							break;

						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					addLog("Possible matches:\n");

					for (int i = 0; i < candidates.size(); i++) {
						addLog("- %s\n", candidates[i]);
					}
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				const int prev_history_pos = historyPos;

				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (historyPos == -1)
						historyPos = (int)history.size() - 1;
					else if (historyPos > 0)
						historyPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (historyPos != -1)
						if (++historyPos >= history.size())
							historyPos = -1;
				}

				if (prev_history_pos != historyPos)
				{
					const char* history_str = (historyPos >= 0) ? history[historyPos] : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
		}

		return 0;
	}

}