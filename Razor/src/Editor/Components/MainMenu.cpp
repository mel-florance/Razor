#include "rzpch.h"
#include "MainMenu.h"
#include "imgui.h"
#include "Razor/Application/Application.h"
#include "ProjectsManager.h"
#include "Editor/Editor.h"
#include "Razor/Core/System.h"
#include "Razor/Core/Engine.h"

#include "Editor/Components/AssetsManager.h"

namespace Razor {

	MainMenu::MainMenu(Editor* editor) : 
		EditorComponent(editor),
		show_create_project(false),
		show_preferences(false)
	{
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::render(float delta)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl + N");
				if (ImGui::IsItemClicked())
					//editor->toggleModal("create_project");
			
				ImGui::MenuItem("Open", "Ctrl + O");

				if (ImGui::BeginMenu("Open Recent...", "Ctrl + Shift + O"))
				{
					ProjectsManager* projManager = (ProjectsManager*) editor->getComponentsManager()->getComponents()["ProjectsManager"];

					if (projManager != nullptr)
					{
						for (auto& proj : projManager->getProjects()) {
							ImGui::MenuItem(proj.name.c_str(), proj.path.c_str());
						}
					}

					ImGui::EndMenu();
				}
			
				ImGui::Separator();

				ImGui::MenuItem("Save", "Ctrl + S");
				ImGui::MenuItem("Save As...", "Ctrl + Shift + S");

				ImGui::Separator();

				ImGui::MenuItem("Preferences");
				show_preferences = ImGui::IsItemClicked();

				ImGui::Separator();

				ImGui::MenuItem("Import...");
				if (ImGui::IsItemClicked())
					//editor->toggleModal("import_asset");

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
				ComponentsManager::ComponentsMap& components = editor->getComponentsManager()->getComponents();
				ComponentsManager::ComponentsMap::iterator it = components.begin();

				std::vector<const char*> filtered = {
					"MainMenu",
					"ProjectsManager",
					"NotificationsManager"
				};

				for (; it != components.end(); ++it)
					if (std::find(filtered.begin(), filtered.end(), it->first) == filtered.end())
						if (ImGui::MenuItem(it->first, nullptr, it->second->isActive()))
							it->second->setActive(!it->second->isActive());

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Manual");
				ImGui::MenuItem("Release notes");
				ImGui::MenuItem("Report a bug");

				ImGui::EndMenu();
			}


			System* system = editor->getEngine()->getSystem();
			system->getStats();
			System::PhysicalMemory& pm = system->getPhysicalMemory();
			std::string size_str = Utils::bytesToSize(pm.process_usage);
			std::string prefix = "Memory usage: ";
			char* total_size = (char*)(prefix + size_str).c_str();
			ImVec2 txt_size = ImGui::CalcTextSize(total_size);

			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - txt_size.x - 15.0f);
			ImGui::Text("%s", total_size);

			ImGui::EndMenuBar();
		}


		
	}


}
