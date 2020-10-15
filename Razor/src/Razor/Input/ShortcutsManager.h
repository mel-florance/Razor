#pragma once

#include "Razor/Events/Event.h"

namespace Razor
{

	class ShortcutsManager
	{
	public:
		ShortcutsManager();
		~ShortcutsManager();

		void onEvent(Event& event);

	private:
		std::unordered_map<std::string, int> shortcuts;
	};

}
