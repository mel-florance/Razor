#include "rzpch.h"
#include "imgui.h"
#include "CreateProject.h"
#include "Editor/Editor.h"
#include "Editor/Components/ProjectsManager.h"

namespace Razor
{

	CreateProject::CreateProject(Editor* editor) : EditorModal(editor)
	{
	}

	CreateProject::~CreateProject()
	{
	}

	void CreateProject::init()
	{
		//projects_manager = editor->getComponent<ProjectsManager*>("ProjectsManager");
	}

	void CreateProject::render()
	{
		if (active)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

			int flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
			if (ImGui::Begin("Create Project", &active, flags))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 120.0f);
					initial_column_spacing++;
				}

				char name[32] = "";
				ImGui::Indent(10.0f);
				ImGui::Text("Project name ");
				ImGui::Indent(-10.0f);

				ImGui::NextColumn();

				ImGui::PushItemWidth(-1);
				ImGui::Indent(10.0f);
				ImGui::InputText("", name, 32);
				ImGui::Indent(-10.0f);
				ImGui::PopItemWidth();

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::Columns(1);

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create project", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					projects_manager->createProject(name);
					active = false;
				}

				ImGui::End();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
	}

}
