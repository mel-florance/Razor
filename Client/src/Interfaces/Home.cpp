#include "Home.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/HomeController.h"

Home::Home() : Interface()
{
}

Home::~Home()
{
}

void Home::render()
{
	auto ctrl = dynamic_cast<HomeController*>(controller);

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(-1, -1));

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 20),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
		IM_COL32(0, 0, 0, 180)
	);

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 90),
		ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 220),
		IM_COL32(255, 50, 50, 150)
	);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font0 = io.Fonts->Fonts[0];
	ImFont* font1 = io.Fonts->Fonts[1];
	ImFont* font2 = io.Fonts->Fonts[8];

	ImGui::SetCurrentFont(font2);
	ImGui::SetCursorPosY(120);
	ImGui::SetCursorPosX(65);
	ImGui::TextColored(ImVec4(100, 100, 100, 0.95f), "RAZOR");
	ImGui::Dummy(ImVec2(0, 20));
	ImGui::SetCursorPosX(40);
	ImGui::TextColored(ImVec4(100, 100, 100, 0.95f), "STRIKES");
	ImGui::SetCurrentFont(font1);

	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 145);

	ImGui::SetCursorPosX(40);
	if (ImGui::Button("MULTIPLAYER", ImVec2(240, 0))) {
		Razor::TestLayer::current_state = Controller::State::GAMES_LIST;
	}

	ImGui::Dummy(ImVec2(-1, 5));
	ImGui::SetCursorPosX(40);
	if (ImGui::Button("OPTIONS", ImVec2(240, 0))) {
		Razor::TestLayer::current_state = Controller::State::OPTIONS;
	}

	ImGui::Dummy(ImVec2(-1, 5));
	ImGui::SetCursorPosX(40);
	if (ImGui::Button("QUIT", ImVec2(240, 0))) {
		Razor::Application::Get().close();
	}

	ImGui::SetCurrentFont(font0);
}
