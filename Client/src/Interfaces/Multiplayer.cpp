#include "Multiplayer.h"
#include "../Client.h"
#include "../Controllers/MultiplayerController.h"

Multiplayer::Multiplayer() :
	Interface(),
	show_game_name_error(false),
	game_name_error("")
{
}

Multiplayer::~Multiplayer()
{
}

void Multiplayer::render()
{
	auto ctrl = dynamic_cast<MultiplayerController*>(controller);

	auto win_pos = ImGui::GetWindowPos();

		//ImGui::Text("Name ");
		//ImGui::PushItemWidth(-1);
		//ImGui::InputText("##nale", input_name, 20);
		//ImGui::PopItemWidth();
		//ImGui::Text("Host ");
		//ImGui::PushItemWidth(-1);
		//ImGui::InputText("##ipaddr", input_host, 15);
		//ImGui::PopItemWidth();
		//ImGui::Text(" Port ");
		//ImGui::PushItemWidth(-1);
		//ImGui::InputText("##port", input_port, 6);
		//ImGui::PopItemWidth();

		//ImGui::Dummy(ImVec2(-1, 10));

		//if (ImGui::Button("Connect", ImVec2(-1, 0))) {
		//
		//}

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(-1, -1));

	ImGui::SetCursorPosX(20);
	ImGui::SetCursorPosY(20);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font0 = io.Fonts->Fonts[0];
	ImFont* font1 = io.Fonts->Fonts[1];
	ImFont* font2 = io.Fonts->Fonts[2];
	ImGui::PushFont(font2);
	ImGui::Text("MULTIPLAYER");
	ImGui::Dummy(ImVec2(0, 15));
	ImGui::PopFont();

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

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
	ImGui::Text("GAME PARAMETERS");
	ImGui::PopFont();

	ImVec2 screen_pos = ImGui::GetCursorScreenPos();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 130),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 131),
		ImColor(255, 255, 255, 100)
	);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(155);

	if (show_game_name_error)
		ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 0.8f), game_name_error);

	ImGui::SetCursorPosX(40);
	ImGui::Text("Game name");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(10, 0));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(140);
	ImGui::InputText("##game_name", ctrl->input_game_name, sizeof(ctrl->input_game_name));
	ImGui::Dummy(ImVec2(0, 10));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Mode");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(52, 0));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(140);

	std::array game_modes = MultiplayerController::game_modes_str();

	if (ImGui::BeginCombo("##game_modes", ctrl->current_game_mode, 0))
	{
		for (int n = 0; n < game_modes.size(); n++)
		{
			bool is_selected = (ctrl->current_game_mode == game_modes[n]);

			if (ImGui::Selectable(game_modes[n], is_selected))
				ctrl->current_game_mode = game_modes[n];

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Dummy(ImVec2(0, 10));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Map");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(60, 0));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(140);

	auto maps_list = MultiplayerController::maps_str();

	if (ImGui::BeginCombo("##maps", ctrl->current_game_map, 0))
	{
		for (int n = 0; n < maps_list.size(); n++)
		{
			bool is_selected = (ctrl->current_game_map == maps_list[n]);

			if (ImGui::Selectable(maps_list[n], is_selected))
				ctrl->current_game_map = maps_list[n];

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Dummy(ImVec2(0, 10));

	ImGui::SetCursorPosX(40);
	ImGui::Text("Max players");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(49, 0));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	ImGui::InputInt("##max_players", &ctrl->input_game_max_players);
	ImGui::Dummy(ImVec2(0, 20));

	ImGui::SetCursorPosX(40);
	if (ImGui::Button("CREATE GAME", ImVec2(240, 0))) {
		if (strlen(ctrl->input_game_name) > 0) {
			show_game_name_error = false;
			ctrl->create_game();
		}
		else {
			std::strncpy(game_name_error, "Please enter a name", sizeof(game_name_error));
			show_game_name_error = true;
		}
	}

	ImGui::SetCursorPosY(88);
	ImGui::NextColumn();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 326, ImGui::GetWindowPos().y + 105),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 25), ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 300),
		IM_COL32(0, 0, 0, 180)
	);

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 280),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 25), ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 220),
		IM_COL32(0, 0, 0, 180)
	);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip;
	if (ImGui::BeginTabBar("tabs", tab_bar_flags))
	{
		bool opened = true;
		ImGuiTabItemFlags flags_item = ImGuiTabItemFlags_NoCloseButton | ImGuiTabItemFlags_Preview;

		ImGui::SetCurrentFont(font0);
		if (ImGui::BeginTabItem(" INTERNET ", &opened, flags_item)) {

			ImVec2 screen_pos = ImGui::GetCursorScreenPos();
			auto win_width = ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 25.0f);

			draw_list->AddRectFilled(
				ImVec2(ImGui::GetWindowPos().x + 325, screen_pos.y + 30.0f),
				ImVec2(win_width, screen_pos.y + 31.0f),
				ImColor(255, 255, 255, 80)
			);

			unsigned short columns = 5;
			ImGui::Columns(columns, "table", true);

			ImGui::SetColumnWidth(0, 650);
			ImGui::SetColumnWidth(1, 200);
			ImGui::SetColumnWidth(2, 200);
			ImGui::SetColumnWidth(3, 100);
			ImGui::SetColumnWidth(4, 100);

			ImGui::Indent(320);

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Text(" GAMES ");
			ImGui::NextColumn();

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Text(" MODE ");
			ImGui::NextColumn();

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Text(" MAP ");
			ImGui::NextColumn();

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Text(" PLAYERS ");
			ImGui::NextColumn();

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Text(" LATENCY ");
			ImGui::NextColumn();

			int hovered = -1;
			int i = 0;


			for (auto& game : ctrl->games) {

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::TextColored(ImVec4(255, 255, 255, 0.5), (' ' + std::string(game.name)).c_str());
				ImGui::NextColumn();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::TextColored(ImVec4(255, 255, 255, 0.5), (' ' + MultiplayerController::game_mode_str(game.mode)).c_str());
				ImGui::NextColumn();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::TextColored(ImVec4(255, 255, 255, 0.5), (' ' + MultiplayerController::get_map_name(game.map)).c_str());
				ImGui::NextColumn();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::TextColored(ImVec4(255, 255, 255, 0.5), (' ' + std::to_string(game.players_count) + " / " + std::to_string(game.max_players)).c_str());
				ImGui::NextColumn();

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::TextColored(ImVec4(255, 255, 255, 0.5), (' ' + std::to_string(game.ping)).c_str());
				ImGui::NextColumn();

				ImGui::Columns(1);

				unsigned int line_height = 25;
				float y_start = ImGui::GetWindowPos().y + 162;

				auto mouse_pos = ImGui::GetMousePos();

				auto base_x = mouse_pos.x - win_pos.x;
				auto base_y = mouse_pos.y - win_pos.y;

				if ((base_x > 325 && base_x < win_width - win_pos.x)
					&& (base_y > 115 + line_height + line_height * i && base_y < 140 + line_height + line_height * i)) {
					y_start = ImGui::GetWindowPos().y + 162;

					if (ImGui::IsMouseDoubleClicked(0)) {
						ctrl->join_game(i);
					}

					hovered = i;
				}
				else {
					hovered = -1;
				}

				//std::cout << hovered << std::endl;

				if (hovered != -1) {
					draw_list->AddRectFilled(
						ImVec2(ImGui::GetWindowPos().x + 325, y_start - line_height + (line_height * hovered)),
						ImVec2(win_width, y_start + (line_height * hovered)),
						ImColor(255, 255, 255, 80)
					);
				}


				ImGui::Columns(columns, std::string("table_" + std::to_string(i)).c_str(), true);

				ImGui::SetColumnWidth(0, 330);
				ImGui::SetColumnWidth(1, 200);
				ImGui::SetColumnWidth(2, 200);
				ImGui::SetColumnWidth(3, 100);
				ImGui::SetColumnWidth(4, 100);

				++i;
			}

			ImGui::Indent(-320);
			ImGui::Columns(1);


			unsigned int j = 0;
			screen_pos = ImGui::GetCursorScreenPos();

			for (auto& game : ctrl->games) {
				auto line = 162 + 25 * j;

				draw_list->AddRectFilled(
					ImVec2(ImGui::GetWindowPos().x + 325, ImGui::GetWindowPos().y + line),
					ImVec2(win_width, ImGui::GetWindowPos().y + line - 1.0f),
					ImColor(255, 255, 255, 80)
				);

				++j;
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" FAVORITES ", &opened, flags_item)) {
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" HISTORY ", &opened, flags_item)) {

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" SPECTATE ", &opened, flags_item)) {

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" LAN ", &opened, flags_item)) {

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" FRIENDS ", &opened, flags_item)) {

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::SetCurrentFont(font0);
	ImGui::Columns(1);

	ImGui::SetCursorPosX(345);
	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 262));

	if (ImGui::Button("REFRESH", ImVec2(160, 0))) {
		ctrl->refresh_games();
	}

	//if (ctrl->connected) {
	//	ImGui::SetCursorPosX(20);
	//	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 60);
	//	ImGui::Text("Connected to %s:%d as %s   Ping %d", ctrl->host, ctrl->port, ctrl->input_name, Razor::TCPClient::ping);
	//}

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 71);

	ImGui::PushFont(font1);

	if (ImGui::Button("BACK", ImVec2(240, 0))) {
		Razor::TestLayer::current_state = Controller::State::MAIN_MENU;
	}

	ImGui::PopFont();
}
