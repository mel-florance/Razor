#include "Lobby.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/LobbyController.h"
#include "../Controllers/MultiplayerController.h"

Lobby::Lobby() : Interface()
{
}

Lobby::~Lobby()
{
}

void Lobby::render()
{
	auto ctrl = dynamic_cast<LobbyController*>(controller);

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(1, -1));

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

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	unsigned short side_panel_width = 320;

	ImGui::Columns(2, "panels", false);
	ImGui::SetColumnWidth(0, 320);
	ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 340);


	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 105),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 400),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(125);

	ImGui::Text("GAME INFOS");
	ImGui::Dummy(ImVec2(0, 16));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(30, 0));
	ImGui::SameLine();
	ImGui::Text(ctrl->current_game_infos.name);

	ImGui::SetCursorPosX(40);
	ImGui::Text("Players");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();
	ImGui::Text((std::to_string(ctrl->current_game_infos.players_count) + " / " + std::to_string(ctrl->current_game_infos.max_players)).c_str());

	ImGui::SetCursorPosX(40);
	ImGui::Text("Mode");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(32, 0));
	ImGui::SameLine();
	ImGui::Text(MultiplayerController::game_mode_str(ctrl->current_game_infos.mode).c_str());

	ImGui::SetCursorPosX(40);
	ImGui::Text("Map");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(40, 0));
	ImGui::SameLine();
	ImGui::Text(MultiplayerController::get_map_name(ctrl->current_game_infos.map).c_str());

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 40, ImGui::GetWindowPos().y + 150),
		ImVec2(ImGui::GetWindowPos().x + 280, ImGui::GetWindowPos().y + 151),
		ImColor(255, 255, 255, 100)
	);

	ImGui::Dummy(ImVec2(0, 16));
	ImGui::SetCursorPosX(40);
	if (ImGui::Button("READY", ImVec2(240, 0))) {
	}

	ImGui::SetCursorPosY(88);
	ImGui::NextColumn();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + 105),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 25), ImGui::GetWindowPos().y + 400),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCurrentFont(font0);
	ImGui::Columns(1);

	ImGui::SetCursorPosX(20);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 36);

	if (ImGui::Button("QUIT LOBBY", ImVec2(240, 0))) {
		//current_state = Controller::State::GAMES_LIST;
		Razor::TestLayer::current_state = Controller::State::GAMES_LIST;
	}
}
