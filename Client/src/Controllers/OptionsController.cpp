#include "OptionsController.h"
#include "../Interfaces/Options.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

using namespace Razor;

OptionsController::OptionsController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::OPTIONS)
{
	this->setInterface(new Options(), textures);
}

OptionsController::~OptionsController()
{
}

void OptionsController::OnEvent(Event& event)
{
}

void OptionsController::OnUpdate(float delta)
{
}

void OptionsController::OnRender()
{
	this->interface->render();
}
