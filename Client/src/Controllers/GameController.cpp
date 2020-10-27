#include "GameController.h"
#include "../Interfaces/Game.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

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
}

void GameController::OnRender()
{
	this->interface->render();
}
