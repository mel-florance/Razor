#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"
#include "Razor/ImGui/ImTextEditor.h"

namespace Razor 
{

	class Editor;

	class RAZOR_API TextEditor : public EditorComponent
	{
	public:
		TextEditor(Editor* editor);
		~TextEditor();
		
		void render(float delta) override;

	private:
		ImTextEditor* text_editor;
		std::string current_filename;
	};

}
