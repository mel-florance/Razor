#include "LobbyController.h"
#include "../Interfaces/Lobby.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"
#include "MultiplayerController.h"
#include "ChatController.h"
#include "Razor/Core/Clock.h"

using namespace Razor;

GameInfo LobbyController::current_game_infos = {};
bool LobbyController::is_player_ready = false;
bool LobbyController::match_started = false;
float LobbyController::match_countdown_total = 3.0f;
float LobbyController::match_countdown_elapsed = 0.0f;
unsigned int LobbyController::countdown_print_status = 3;
std::unique_ptr<Razor::Clock> LobbyController::match_countdown = std::make_unique<Razor::Clock>();

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
	if (match_started) {
		match_countdown_elapsed += delta;

		auto rounded = std::ceil(match_countdown_elapsed);
		auto ctrl = TestLayer::getController<ChatController>("chat");

		if (rounded == 1 && countdown_print_status == 3) {
			ctrl->add_message("[SERVER]", "Starting in 3...");
			countdown_print_status = 2;
		}
		else if (rounded == 2 && countdown_print_status == 2) {
			ctrl->add_message("[SERVER]", "Starting in 2...");
			countdown_print_status = 1;
		}
		else if (rounded == 3 && countdown_print_status == 1) {
			ctrl->add_message("[SERVER]", "Starting in 1...");
			countdown_print_status = 0;
		}
		else if (match_countdown_elapsed >= match_countdown_total) {
			ctrl->add_message("[SERVER]", "Match started !");
			Razor::TestLayer::current_state = Controller::State::LOADING;
			match_started = false;
		}
	}
}

void LobbyController::OnRender()
{
	this->interface->render();
}

void LobbyController::set_player_ready()
{
	auto packet = Packet::create<MarkPlayerReady>(TCPClient::session_token);
	packet.gameId = current_game_infos.gameId;
	packet.ready = !is_player_ready;

	client->emit(client->handle, packet);
}

void LobbyController::leave_lobby()
{
	auto packet = Packet::create<LeaveGame>(TCPClient::session_token);
	packet.gameId = current_game_infos.gameId;
	client->emit(client->handle, packet);
}

void LobbyController::onPlayerReady(Packet* packet)
{
	auto chat = TestLayer::getController<ChatController>("chat");
	auto request = reinterpret_cast<PlayerReady*>(packet);

	if (request != nullptr) {
		for (auto& player : current_game_infos.players.players) {
			if (player.userId == request->userId) {

				if (player.userId == MultiplayerController::session_user_id)
					is_player_ready = request->ready;

				player.ready = request->ready;
				break;
			}
		}
	}
}

void LobbyController::onGameCreated(Packet* packet)
{
	std::cout << "ON_GAME_CREATED: " << packet->to_string() << std::endl;

	auto request = reinterpret_cast<GameCreated*>(packet);

	if (request != nullptr) {
		auto infos = reinterpret_cast<GameInfo*>(&request->infos);

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

void LobbyController::onPlayerStrangerJoined(Razor::Packet* packet)
{
	auto request = reinterpret_cast<PlayerStrangerJoined*>(packet);
	std::cout << "STRANGER JOINED: " << request->userId << std::endl;

	if (request != nullptr) {
		auto infos = reinterpret_cast<GameInfo*>(&request->info);
		current_game_infos = *infos;
		tm* tm_local = Utils::getLocalTime();
		auto ctrl = TestLayer::getController<ChatController>("chat");

		for (auto& p : current_game_infos.players.players) {
			if (p.userId == request->userId) {
				ctrl->add_message(p.username, "has joined the lobby");
				return;
			}
		}
	}
}

void LobbyController::onPlayerSelfJoined(Razor::Packet* packet)
{
	auto request = reinterpret_cast<PlayerSelfJoined*>(packet);

	if (request != nullptr) {
		auto infos = reinterpret_cast<GameInfo*>(&request->info);

		current_game_infos = *infos;
		TestLayer::current_state = Controller::State::LOBBY;
	}
}

void LobbyController::onPlayerLeaved(Razor::Packet* packet)
{
	auto request = reinterpret_cast<PlayerLeaved*>(packet);

	if (request != nullptr) {

		auto infos = reinterpret_cast<PlayerInfo*>(&request->info);

		if (infos != nullptr) {

			std::cout << "infos: " << infos->userId << std::endl;
			tm* tm_local = Utils::getLocalTime();
			auto ctrl = TestLayer::getController<ChatController>("chat");

			for (auto& p : current_game_infos.players.players) {

				if (p.userId == infos->userId) {
					ctrl->add_message(p.username, "has leaved the lobby");

					PlayerInfo inf;

					inf.userId = 0;
					inf.ready = false;
					std::strncpy(inf.username, "\0", sizeof(PlayerInfo::username));
					p = inf;

					current_game_infos.players_count--;

					break;
				}
			}
		}
	}
}

void LobbyController::onMatchReady(Razor::Packet* packet)
{
	auto request = reinterpret_cast<MatchReady*>(packet);

	if (request != nullptr) {
		match_started = true;
	}
}
