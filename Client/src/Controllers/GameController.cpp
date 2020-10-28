#include "GameController.h"
#include "../Interfaces/Game.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Core/Viewport.h"

using namespace Razor;

GameController::GameController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::GAME)
{
	this->setInterface(new Game(), textures);
}

GameController::~GameController()
{
}

void GameController::OnEvent(Event& event)
{

}

void GameController::OnUpdate(float delta)
{
	Application::Get().getScenesManager()->getActiveScene()->getActiveCamera()->getViewport()->setHovered(true);
}

void GameController::OnRender()
{
	this->interface->render();
}
