#include "rzpch.h"
#include "MainMenu.h"
#include "imgui.h"
#include "Razor/Application.h"

namespace Razor {

	MainMenu::MainMenu()
	{
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::setup()
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
				ImGui::MenuItem("Cut", "Ctrl + X", false, true);
				ImGui::MenuItem("Copy", "Ctrl + C", false, true);
				ImGui::MenuItem("Paste", "Ctrl + V", false, true);
				ImGui::MenuItem("Delete", "Del", false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Toggle Console", NULL, false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Manual", NULL, false, true);
				ImGui::MenuItem("Release notes", NULL, false, true);
				ImGui::MenuItem("Report a bug", NULL, false, true);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

}
