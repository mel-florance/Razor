#pragma once

#include "imgui.h"
#include "Razor/Core/Core.h"
#include "Razor/Filesystem/FileWatcher.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API Logger : public EditorComponent
	{
	public:
		Logger(Editor* editor);
		~Logger();

		void clear();
		void addLog(const char* fmt, ...) IM_FMTARGS(2);
		void render() override;

	private:
		ImGuiTextBuffer buffer;
		ImGuiTextFilter filter;
		ImVector<int> lineOffsets;
		bool autoScroll;
		bool scrollToBottom;
	};

}
