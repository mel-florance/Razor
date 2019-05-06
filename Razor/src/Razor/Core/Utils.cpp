#include "rzpch.h"
#include "Transform.h"
#include <imgui.h>

namespace Razor
{

	bool Utils::treeNode(const char* label)
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();
		ImU32 id = ImGui::GetID(label);
		int opened = storage->GetInt(id, 0);
		float x = ImGui::GetCursorPosX();

		ImGui::BeginGroup();
	
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));

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
				ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
				ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y),
				ImColor(ImVec4(0.3f, 0.3f, 0.3f, 0.7f)));
		}
		else
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
				ImVec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y),
				ImColor(ImVec4(0.2f, 0.2f, 0.2f, 0.7f)));


		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 3.0f, ImGui::GetCursorPosY() - 27.0f));
		ImGui::ArrowButton("##arrow_button", opened ? ImGuiDir_Down : ImGuiDir_Right);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY()));
		ImGui::Text(label);
		ImGui::EndGroup();

		if (opened)
			ImGui::TreePush(label);

		return opened != 0;
	}

}

