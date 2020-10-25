#include "Chat.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/LobbyController.h"
#include "../Controllers/MultiplayerController.h"
#include "../Controllers/ChatController.h"

Chat::Chat() : Interface()
{
}

Chat::~Chat()
{
}

void Chat::render()
{
	auto ctrl = dynamic_cast<ChatController*>(controller);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("#test", ImVec2(0, 285), false, ImGuiWindowFlags_NoBackground);

	ImGui::SetCursorPosY(10);

	for (auto& message : ctrl->messages) {
		if (message.text.size() > 0) {
			ImGui::SetCursorPosX(20);
			ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 4.0f), message.time.c_str());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 5.0f), message.username.c_str());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), message.text.c_str());
		}
	}

	if (ctrl->messages.size() > 0) {
		auto last = &ctrl->messages.back();

		if (last != NULL) {
			if (ctrl->last_message!= last) {
				ImGui::SetScrollHere(0.f);
				ctrl->last_message = last;
			}
		}
	}

	ImGui::Dummy(ImVec2(0, 10));

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + 285 + 130),
		ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 25, ImGui::GetWindowPos().y + 285 + 131),
		ImColor(255, 255, 255, 100)
	);

	ImGui::PushItemWidth(ImGui::GetItemRectSize().x - 40);
	ImGui::SetCursorPosX(348);
	ImGui::SetCursorPosY(285 + 131 + 20);

	if (ImGui::InputText("##message", ctrl->input_message, 128, ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (strlen(ctrl->input_message) > 0) {
			ctrl->send();
		}

		ImGui::SetKeyboardFocusHere(0);
	}

	ImGui::PopItemWidth();
}
