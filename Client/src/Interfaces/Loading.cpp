#include "Loading.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/LoadingController.h"

Loading::Loading() : Interface()
{
}

Loading::~Loading()
{
}

void Loading::render()
{
	auto ctrl = Razor::TestLayer::getController<LoadingController>("loading");

	ImGui::Image((void*)textures["background"]->getId(), ImGui::GetWindowSize(), ImVec2(), ImVec2(-1, -1));

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(
		ImVec2(ImGui::GetWindowPos().x + 20, ImGui::GetWindowPos().y + 20),
		ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 20, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 20),
		IM_COL32(0, 0, 0, 180)
	);

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorPosY(40);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font2 = io.Fonts->Fonts[2];
	ImGui::PushFont(font2);
	ImGui::Text("LOADING");
	ImGui::PopFont();

	ImGui::SetCursorPosX(40);
	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 65));
	ImGui::ProgressBar(ctrl->progress, ImVec2(ImGui::GetWindowWidth() - 80.0f, 0.0f));
}
