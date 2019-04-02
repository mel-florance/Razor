#include "rzpch.h"
#include "MainMenu.h"
#include "imgui.h"
#include "Razor/Application.h"

namespace Razor {

	MainMenu::MainMenu(Editor* editor) : EditorComponent(editor)
	{
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::render()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl + N");
				ImGui::MenuItem("Open", "Ctrl + O");
				ImGui::MenuItem("Open Recent...", "Ctrl + Shift + O");
				ImGui::Separator();
				ImGui::MenuItem("Save", "Ctrl + S");
				ImGui::MenuItem("Save As...", "Ctrl + Shift + S");
				ImGui::Separator();
				ImGui::MenuItem("Preferences");
				ImGui::Separator();
				ImGui::MenuItem("Import...");
				ImGui::MenuItem("Export...");
				ImGui::Separator();

				ImGui::MenuItem("Quit", "Ctrl + Q");

				if (ImGui::IsItemClicked())
					Application::Get().close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Cut", "Ctrl + X");
				ImGui::MenuItem("Copy", "Ctrl + C");
				ImGui::MenuItem("Paste", "Ctrl + V");
				ImGui::MenuItem("Delete", "Del");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Assets Manager");
				ImGui::MenuItem("Console");
				ImGui::MenuItem("Outliner");
				ImGui::MenuItem("Properties Editor");
				ImGui::MenuItem("Tools");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Manual");
				ImGui::MenuItem("Release notes");
				ImGui::MenuItem("Report a bug");

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

}
