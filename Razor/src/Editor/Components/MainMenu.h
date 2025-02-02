#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API MainMenu : public EditorComponent
	{
	public:
		MainMenu(Editor* editor);
		~MainMenu();

		void render(float delta) override;

		bool show_create_project;
		bool show_preferences;

	private:
	};

}
