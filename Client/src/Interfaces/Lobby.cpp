#include "Lobby.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/LobbyController.h"
#include "../Controllers/MultiplayerController.h"
#include "../Controllers/ChatController.h"

Lobby::Lobby() :
	Interface(),
	ready_button_hovered(false)
{
}

Lobby::~Lobby()
{
}

void Lobby::render()
{
	auto ctrl = dynamic_cast<LobbyController*>(controller);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(-1, -1));

	ImGui::SetCursorPosX(20);
	ImGui::SetCursorPosY(20);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font0 = io.Fonts->Fonts[0];
	ImFont* font1 = io.Fonts->Fonts[1];
	ImFont* font2 = io.Fonts->Fonts[2];
	ImGui::PushFont(font2);
	ImGui::Text("LOBBY");
	ImGui::Dummy(ImVec2(0, 15));
	ImGui::PopFont();

	unsigned short side_panel_width = 320;

	ImGui::Columns(2, "panels", false);
	ImGui::SetColumnWidth(0, 320);
	ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 340);

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 80),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(95);
	ImGui::PushFont(font1);
	ImGui::Text("GAME INFOS");
	ImGui::PopFont();

	ImGui::Dummy(ImVec2(0, 30));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(30, 0));
	ImGui::SameLine();
	ImGui::Text(ctrl->current_game_infos.name);

	ImGui::Dummy(ImVec2(0, 3));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Players");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();
	ImGui::Text((std::to_string(ctrl->current_game_infos.players_count) + " / " + std::to_string(ctrl->current_game_infos.max_players)).c_str());

	ImGui::Dummy(ImVec2(0, 3));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Mode");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(32, 0));
	ImGui::SameLine();
	ImGui::Text(MultiplayerController::game_mode_str(ctrl->current_game_infos.mode).c_str());

	ImGui::Dummy(ImVec2(0, 3));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Map");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(40, 0));
	ImGui::SameLine();
	ImGui::Text(MultiplayerController::get_map_name(ctrl->current_game_infos.map).c_str());

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 130),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 131),
		ImColor(255, 255, 255, 100)
	);

	ImGui::Dummy(ImVec2(0, 16));
	ImGui::SetCursorPosX(40);

	ImGui::PushFont(font1);

	if (ctrl->is_player_ready) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 100, 0, 0.7));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(100, 0, 0, 0.8));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(100, 0, 0, 0.9));
	}

	if (ImGui::Button(ready_button_hovered && ctrl->is_player_ready ? "NOT READY" : "READY", ImVec2(240, 0))) {
		ctrl->set_player_ready();
	}

	ready_button_hovered = ImGui::IsItemHovered();

	if (ctrl->is_player_ready) {
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	ImGui::PopFont();

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(340);
	ImGui::PushFont(font1);
	ImGui::Text("PLAYERS");
	ImGui::PopFont();

	ImGui::Dummy(ImVec2(0, 30));

	ImGui::SetCursorPosX(40);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.5));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 10));

	ImGui::BeginChild("#players_list", ImVec2(240, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() - 880), false, 0);

	ImGui::SetCursorPosX(10);
	ImGui::SetCursorPosY(6);

	for (auto& player : ctrl->current_game_infos.players.players) {
		if (strlen(player.username) > 0) {

			if (player.ready) {
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 100, 0, 0.7));
			}

			ImGui::SetCursorPosX(10);
			ImGui::Selectable(player.username, player.ready, 0, ImVec2(220, 0));

			if (player.ready) {
				ImGui::PopStyleColor();
			}
		}
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 370),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 371),
		ImColor(255, 255, 255, 100)
	);

	ImGui::SetCursorPosY(25);
	ImGui::NextColumn();
	
	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + 82),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 25), ImGui::GetWindowPos().y + 485),
		IM_COL32(0, 0, 0, 180)
	);

	auto chat = Razor::TestLayer::getController<ChatController>("chat");

	if (chat != nullptr) {
		ImGui::SetCursorPosX(340);
		ImGui::SetCursorPosY(95);
		ImGui::PushFont(font1);
		ImGui::Text("CHAT");

		ImGui::PopFont();
		draw_list->AddRectFilled(
			ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 80),
			ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
			IM_COL32(0, 0, 0, 180)
		);

		draw_list->AddRectFilled(
			ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + 130),
			ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 25, ImGui::GetWindowPos().y + 131),
			ImColor(255, 255, 255, 100)
		);

		ImGui::SetCursorPosY(131);
		chat->interface->render();
	}

	ImGui::SetCurrentFont(font0);
	ImGui::Columns(1);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 71);

	ImGui::PushFont(font1);

	if (ImGui::Button("QUIT LOBBY", ImVec2(240, 0))) {

		ctrl->leave_lobby();
		ctrl->is_player_ready = false;
		ChatController::messages.clear();

		Razor::TestLayer::current_state = Controller::State::GAMES_LIST;
	}

	ImGui::PopFont();
}
