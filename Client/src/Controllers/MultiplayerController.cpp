#include "MultiplayerController.h"
#include "../Interfaces/Lobby.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

std::vector<Razor::GameInfo> MultiplayerController::games = {};
bool MultiplayerController::connected = false;
uint32_t MultiplayerController::session_user_id = 0;
char MultiplayerController::session_token[64] = {};

using namespace Razor;

const std::unordered_map<uint32_t, std::string> MultiplayerController::maps = {
	{0, "Hangar"},
	{1, "City"},
	{2, "Port"},
	{3, "Building"},
};

MultiplayerController::MultiplayerController(
	std::shared_ptr<TCPClient> client, 
	const std::unordered_map<std::string, Texture*>& textures
) :
	Controller(client, State::GAMES_LIST),
	input_name("Guest"),
	input_game_mode(GameMode::CASUAL),
	current_game_mode("Casual"),
	input_game_map(0),
	current_game_map("Hangar"),
	input_game_max_players(10),
	refreshing_server_list(false),
	port(55555),
	host("127.0.0.1")
{
	this->setInterface(new Multiplayer(), textures);
}

MultiplayerController::~MultiplayerController()
{
}

void MultiplayerController::OnEvent(Razor::Event& event)
{
}

void MultiplayerController::OnUpdate(float delta)
{
	if (!connected) {
		connected = client->Connect(host, port);

		if (connected) {
			login();
			Sleep(100);
			refresh_games();
		}
	}
}

void MultiplayerController::OnRender()
{
	interface->render();
}

void MultiplayerController::login()
{
	auto packet = Razor::Packet::create<LoginRequest>();

	std::strncpy(packet.username, input_name, sizeof(LoginRequest::username));
	std::strncpy(packet.password, "test", sizeof(LoginRequest::password));

	client->emit(client->handle, packet);
}

void MultiplayerController::create_game()
{
	auto packet = Razor::Packet::create<CreateGame>();

	std::strncpy(packet.name, input_game_name, sizeof(CreateGame::name));
	packet.max_players = input_game_max_players;
	packet.mode = get_mode_from_name(current_game_mode);
	packet.map = get_map_from_name(current_game_map);

	std::cout << packet.name << std::endl;
	std::cout << packet.max_players << std::endl;
	std::cout << packet.mode << std::endl;
	std::cout << packet.map << std::endl;

	client->emit(client->handle, packet);
	memset(input_game_name, 0, sizeof(input_game_name));
}

void MultiplayerController::refresh_games()
{
	auto packet = Razor::Packet::create<RefreshGamesList>();
	client->emit(client->handle, packet);
	refreshing_server_list = true;
}

void MultiplayerController::join_game(uint32_t gameId)
{
	auto packet = Razor::Packet::create<Razor::JoinGame>();
	packet.gameId = gameId;
	client->emit(client->handle, packet);
}

void MultiplayerController::set_default_game_name() {
	std::strncpy(input_game_name, (std::string(input_name) + "'s game").c_str(), sizeof(MultiplayerController::input_game_name));
}

void MultiplayerController::onLoginResponse(Razor::Packet* packet)
{
	auto response = (Razor::LoginResponse*)packet;

	session_user_id = response->userId;
	std::strncpy(session_token, response->token, sizeof(TCPClient::session_token));

	Log::trace("User logged: %s", std::to_string(response->userId).c_str());
}

void MultiplayerController::onGameDestroyed(Packet* packet)
{
	auto game_infos = reinterpret_cast<GameDestroyed*>(packet);
	std::cout << "RECEIVE GAME DESTROY" << std::endl;

	if (game_infos != nullptr) {
		std::vector<GameInfo>::iterator it = games.begin();

		for (; it != games.end();) {
			if (it->gameId == game_infos->gameId) {
				it = games.erase(it);
				Log::trace("Game %d destroyed", game_infos->gameId);
			}
			else {
				it++;
			}
		}
	}
}

void MultiplayerController::onGamesList(Packet* packet)
{
	auto list = reinterpret_cast<GamesList*>(packet);

	std::cout << "GAMES LIST: " << std::endl;

	for (auto game : list->games) {
		auto info = reinterpret_cast<GameInfo* > (&game);

		if (info != nullptr) {

			if (strlen(info->name) > 0) {
				std::cout << "INFO NAME: " << info->name << std::endl;

				auto it = std::find_if(games.begin(), games.end(),
					[=](const GameInfo& game_info) {
						return game_info.gameId == info->gameId;
					});

				if (it != games.end()) {
					*it = *info;
				}
				else {
					games.push_back(*info);
				}
			}
		}
	}
}
