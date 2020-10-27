#include "LoadingController.h"
#include "../Interfaces/Loading.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

using namespace Razor;

LoadingController::LoadingController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::GAME)
{
	this->setInterface(new Loading(), textures);
}

LoadingController::~LoadingController()
{
}

void LoadingController::OnEvent(Event& event)
{
}

void LoadingController::OnUpdate(float delta)
{
}

void LoadingController::OnRender()
{
	this->interface->render();
}
