#include "rzpch.h"
#include "TextEditor.h"
#include <imgui.h>

namespace Razor
{

	TextEditor::TextEditor(Editor* editor) :
		EditorComponent(editor),
		current_filename("Untitled")
	{
		text_editor = new ImTextEditor();

		auto lang = ImTextEditor::LanguageDefinition::GLSL();
		text_editor->SetLanguageDefinition(lang);
		text_editor->SetTabSize(4);
		text_editor->SetShowWhitespaces(false);

		active = false;
	}

	TextEditor::~TextEditor()
	{
	}

	void TextEditor::render(float delta)
	{
		bool open;
		int flags = 
			ImGuiWindowFlags_MenuBar    | 
			ImGuiWindowFlags_NoNavFocus | 
			ImGuiWindowFlags_NoNav      | 
			ImGuiWindowFlags_NoNavInputs;

		ImGui::Begin("Text editor", &open, flags);

		bool ro = text_editor->IsReadOnly();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl + N");
				ImGui::Separator();
				ImGui::MenuItem("Open file...", "Ctrl + O");
				ImGui::Separator();
				ImGui::MenuItem("Save", "Ctrl + S");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Undo", "Ctrl + Z", nullptr, !ro && text_editor->CanUndo());
				ImGui::MenuItem("Redo", "Ctrl + Y", !ro && text_editor->CanRedo());

				ImGui::Separator();

				if (ImGui::MenuItem("Cut", "Ctrl + X", !ro && text_editor->HasSelection()))
					text_editor->Cut();
				if (ImGui::MenuItem("Copy", "Ctrl + C", nullptr, text_editor->HasSelection()))
					text_editor->Copy();
				if (ImGui::MenuItem("Paste", "Ctrl + V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					text_editor->Paste();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor->HasSelection()))
					text_editor->Delete();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Selection"))
			{
				if(ImGui::MenuItem("Select all", "Ctrl + A", nullptr))
					text_editor->SetSelection(ImTextEditor::Coordinates(), ImTextEditor::Coordinates(text_editor->GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Language"))
			{
				if (ImGui::MenuItem("AngelScript")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::AngelScript());
				}
				if (ImGui::MenuItem("C")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::C());
				}
				if (ImGui::MenuItem("C++")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::CPlusPlus());
				}
				if (ImGui::MenuItem("GLSL")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::GLSL());
				}
				if (ImGui::MenuItem("HLSL")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::HLSL());
				}
				if (ImGui::MenuItem("Lua")) {
					text_editor->SetLanguageDefinition(ImTextEditor::LanguageDefinition::Lua());
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette", nullptr, text_editor->GetPalette() == ImTextEditor::GetDarkPalette()))
					text_editor->SetPalette(ImTextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette", nullptr, text_editor->GetPalette() == ImTextEditor::GetLightPalette()))
					text_editor->SetPalette(ImTextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette", nullptr, text_editor->GetPalette() == ImTextEditor::GetRetroBluePalette()))
					text_editor->SetPalette(ImTextEditor::GetRetroBluePalette());

				ImGui::Separator();
				
				if (ImGui::MenuItem("Show white spaces"))
					text_editor->SetShowWhitespaces(!text_editor->IsShowingWhitespaces());
				
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints(ImVec2(), ImVec2(size.x, size.y - 75));

		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->Fonts[1];

		ImGui::PushFont(font);
		text_editor->Render("TextEditor");
		ImGui::PopFont();

		auto cpos = text_editor->GetCursorPosition();
		ImGui::Dummy(ImVec2(0, 3));
		int nb_lines = text_editor->GetTotalLines();
		const char* lines_plural = nb_lines > 1 ? "lines" : "line";

		ImGui::Text("%6d/%-6d %6d %s  | %s | %s | %s %s",
			cpos.mLine + 1,
			cpos.mColumn + 1,
			nb_lines,
			lines_plural,
			text_editor->IsOverwrite() ? "Ovr" : "Ins",
			text_editor->GetLanguageDefinition().mName.c_str(), 
			current_filename.c_str(),
			text_editor->CanUndo() ? "*" : " "
		);
	
		ImGui::End();
	}

}
