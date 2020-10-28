#include "Game.h"
#include "../Controller.h"
#include "../Client.h"
#include "../Controllers/GameController.h"
#include "Razor/Application/Application.h"
#include "Razor/Rendering/Renderer.h"
#include "Razor/Core/Engine.h"
#include "Razor/Application/Application.h"
#include "Razor/Scene/ScenesManager.h"
#include <GLFW/glfw3.h>
using namespace Razor;

Game::Game() :
	Interface(),
	current_pass(0)
{
	Renderer* renderer = Application::Get().getEngine()->getRenderer();
	auto g_buffer = renderer->getGBuffer();
	current_pass = g_buffer->getCombined();
}

Game::~Game()
{
}

void Game::render()
{
	ImGui::Image((void*)(intptr_t)current_pass, ImGui::GetWindowSize());
}
