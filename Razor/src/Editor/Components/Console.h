#pragma once

#include "imgui.h"
#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
	static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

	class RAZOR_API Console : public EditorComponent
	{
	public:
		Console(Editor* editor);
		~Console();

		void render(float delta) override;
		void clearLog();
		void addLog(const char* fmt, ...) IM_FMTARGS(2);
		void execCommand(const char* command_line);

		int TextEditCallback(ImGuiInputTextCallbackData* data);

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
		{
			Console* console = (Console*)data->UserData;
			return console->TextEditCallback(data);
		}

	private:
		char input[256];

		std::vector<char*> items;
		std::vector<const char*> commands;
		std::vector<char*> history;
		int historyPos;
		ImGuiTextFilter filter;
		bool autoScroll;
		bool scrollToBottom;
	};

}
