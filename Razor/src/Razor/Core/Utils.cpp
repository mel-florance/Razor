#include "rzpch.h"
#include "Transform.h"
#include <imgui.h>
#include "Editor/Editor.h"

namespace Razor
{

	bool Utils::treeNode(
		const char* label,
		bool has_icon,
		bool stripe,
		const std::string& icon_name,
		const std::string& icon_hover,
		bool selected
	)
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();
		ImU32 id = ImGui::GetID(label);
		int opened = storage->GetInt(id, 0);
		float x = ImGui::GetCursorPosX();

		ImGui::BeginGroup();
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 8));

		if (ImGui::InvisibleButton(label, ImVec2(-1, ImGui::GetFontSize() + style.FramePadding.y * 2.0f)))
		{
			int* p_opened = storage->GetIntRef(id, 0);
			opened = *p_opened = !*p_opened;
		}
	
		ImGui::PopStyleVar();

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();

		if (hovered || active)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(ImGui::GetItemRectMin().x - 32.0f, ImGui::GetItemRectMin().y),
				ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y),
				ImColor(ImVec4(0.3f, 0.3f, 0.3f, 0.7f)));
		}

		if (selected)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(ImGui::GetItemRectMin().x - 32.0f, ImGui::GetItemRectMin().y),
				ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y),
				ImColor(ImVec4(0.4f, 0.4f, 0.4f, 0.7f)));
		}

		if (stripe)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(ImGui::GetItemRectMin().x - 32.0f, ImGui::GetItemRectMin().y),
				ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y + 1.0f),
				ImColor(ImVec4(0.2f, 0.2f, 0.2f, 0.3f)));
		}

		float margin = 1.0f;

		if (has_icon) 
		{
			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 23.0f));

			if (icon_name == "arrow_button") 
				ImGui::ArrowButton("##arrow_button", opened ? ImGuiDir_Down : ImGuiDir_Right);
			else 
			{
				Editor::drawIcon((hovered || selected) && !icon_hover.empty() ? icon_hover : icon_name, glm::vec2(20, 20));
				margin = -3.0f;
			}
	
			ImGui::SameLine();
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - margin));
		ImGui::TextColored(selected ? ImColor(ImVec4(1.0f, 0.607f, 0.176f, 0.8f)) : ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.8f)), label);
		ImGui::EndGroup();

		if (opened)
			ImGui::TreePush(label);

		return opened != 0;
	}

}

