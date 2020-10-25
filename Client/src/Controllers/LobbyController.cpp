#include "LobbyController.h"
#include "../Interfaces/Lobby.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"
#include "MultiplayerController.h"

using namespace Razor;

Razor::GameInfo LobbyController::current_game_infos = {};

LobbyController::LobbyController(
	std::shared_ptr<TCPClient> client,
	const std::unordered_map<std::string, Texture*>& textures
) :
	Controller(client, State::LOBBY)
{
	this->setInterface(new Lobby(), textures);
}

LobbyController::~LobbyController()
{
}

void LobbyController::OnEvent(Event& event)
{
}

void LobbyController::OnUpdate(float delta)
{
}

void LobbyController::OnRender()
{
	this->interface->render();
}


void LobbyController::set_player_ready()
{
	auto packet = Packet::create<MarkPlayerReady>(TCPClient::session_token);
	client->emit(client->handle, packet);
}

void LobbyController::onPlayerReady(Razor::Packet* packet)
{
	auto player = reinterpret_cast<PlayerReady*>(packet);

	if (player != nullptr) {
		std::cout << "PLAYER READY: " << player->userId << std::endl;
	}
}

void LobbyController::onGameCreated(Packet* packet)
{
	std::cout << "ON_GAME_CREATED: " << packet->to_string() << std::endl;

	auto game_infos = reinterpret_cast<GameCreated*>(packet);

	if (game_infos != nullptr) {
		auto infos = reinterpret_cast<GameInfo*>(&game_infos->infos);

		auto ctrl = TestLayer::getController<MultiplayerController>("multiplayer");
		ctrl->games.push_back(*infos);

		if (ctrl->session_user_id == infos->creator) {
			current_game_infos = *infos;
			
			std::cout << "Received new game infos:" << std::endl;
			std::cout << "------------------------" << std::endl;
			std::cout << "Name: " << infos->name<< std::endl;
			std::cout << "GameId: " << infos->gameId << std::endl;
			std::cout << "Nb Players: " << infos->players_count << std::endl;
			std::cout << "Max Players: " << infos->max_players << std::endl;
			std::cout << "Ping: " << infos->ping << std::endl;

			TestLayer::current_state = Controller::State::LOBBY;
		}
	}
}
