#include "Options.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/OptionsController.h"

Options::Options() :
	Interface(),
	sections({
		"GENERAL",
		"VIDEO",
		"AUDIO",
		"GAMEPLAY",
		"CONTROLS"
	}),
	selected_section("GENERAL")
{
}

Options::~Options()
{
}

void Options::render()
{
	auto ctrl = dynamic_cast<OptionsController*>(controller);

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(-1, -1));

	ImGui::SetCursorPosX(20);
	ImGui::SetCursorPosY(20);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font0 = io.Fonts->Fonts[0];
	ImFont* font1 = io.Fonts->Fonts[1];
	ImFont* font2 = io.Fonts->Fonts[2];
	ImGui::PushFont(font2);
	ImGui::Text("OPTIONS");
	ImGui::Dummy(ImVec2(0, 15));
	ImGui::PopFont();

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::Columns(2, "panels_options", false);
	ImGui::SetColumnWidth(0, 320);
	ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 20);

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 80),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(105);

	for (auto& section : sections)
	{
		ImGui::SetCursorPosX(50);
		ImGui::PushFont(font1);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 10));

		if (ImGui::Selectable(section, selected_section == section, 0, ImVec2(320 - 100, 0)))
			selected_section = section;

		ImGui::PopStyleVar();
		ImGui::PopFont();
	}

	ImGui::SetCursorPosY(87);
	ImGui::NextColumn();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 320, ImGui::GetWindowPos().y + 83),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 20), ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCursorPosY(103);
	ImGui::SetCursorPosX(340);
	ImGui::PushFont(font1);
	ImGui::Text(selected_section);
	ImGui::PopFont();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 320, ImGui::GetWindowPos().y + 133),
		ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 20), ImGui::GetWindowPos().y + 134),
		IM_COL32(255, 255, 255, 50)
	);

	float right_panel = ImGui::GetWindowWidth() - 380.0f;

	ImGui::SetCursorPosY(163);
	ImGui::SetCursorPosX(340);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("#test", ImVec2(right_panel, ImGui::GetWindowHeight() - 203.0f), false, ImGuiWindowFlags_NoBackground);

	if (selected_section == "GENERAL")
		display_general();
	else if (selected_section == "VIDEO")
		display_video();
	else if (selected_section == "AUDIO")
		display_audio();
	else if (selected_section == "GAMEPLAY")
		display_gameplay();
	else if (selected_section == "CONTROLS")
		display_controls();

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::Columns(1);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 71);

	ImGui::PushFont(font1);

	if (ImGui::Button("BACK", ImVec2(240, 0))) {
		Razor::TestLayer::current_state = Controller::State::MAIN_MENU;
	}

	ImGui::PopFont();
}

void Options::display_general()
{
	ImGui::Columns(2, "properties", false);
	ImGui::SetColumnWidth(0, 320.0f);
	ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 380.0f - 320.0f);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->Fonts[1];
	ImGui::PushFont(font1);
	ImGui::Text("LANGUAGE");
	ImGui::PopFont();

	ImGui::NextColumn();

	ImGui::Text("test");

	ImGui::Columns(1);
}

void Options::display_video()
{
}

void Options::display_audio()
{
}

void Options::display_gameplay()
{
}

void Options::display_controls()
{
}
