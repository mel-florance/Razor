#include "HomeController.h"
#include "../Interfaces/Home.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

using namespace Razor;

HomeController::HomeController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::MAIN_MENU)
{
	this->setInterface(new Home(), textures);
}

HomeController::~HomeController()
{
}

void HomeController::OnEvent(Event& event)
{
}

void HomeController::OnUpdate(float delta)
{
}

void HomeController::OnRender()
{
	this->interface->render();
}
