#include "rzpch.h"
#include "imgui.h"
#include "ImportAsset.h"
#include "Editor/Editor.h"

namespace Razor
{

	ImportAsset::ImportAsset(Editor* editor) : EditorModal(editor)
	{
	}

	ImportAsset::~ImportAsset()
	{
	}

	void ImportAsset::render()
	{
		if (active)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

			if (ImGui::Begin("Import options", &active, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 120.0f);
					initial_column_spacing++;
				}

				ImGui::Text("ok");

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::Columns(1);

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Import", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				ImGui::End();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
	}

}