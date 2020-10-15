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
		void addLog(const std::string& str);
		void render(float delta) override;
		void onEvent(Event& event) override;

	private:
		bool hovered;
		bool autoScroll;
		bool scrollToBottom;

		std::vector<std::string> logs;
	};

}
