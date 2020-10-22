#include "MultiplayerController.h"
#include "../Interfaces/Lobby.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"

std::vector<Razor::GameInfo> MultiplayerController::games = {};
bool MultiplayerController::connected = false;

const std::unordered_map<uint32_t, std::string> MultiplayerController::maps = {
	{0, "Hangar"},
	{1, "City"},
	{2, "Port"},
	{3, "Building"},
};

MultiplayerController::MultiplayerController(
	std::shared_ptr<Razor::TCPClient> client, 
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::GAMES_LIST),
	input_name("Guest"),
	input_game_name("Guest's game"),
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
	auto packet = Razor::Packet::create<Razor::LoginRequest>();

	std::strncpy(packet.username, input_name, sizeof(Razor::LoginRequest::username));
	std::strncpy(packet.password, "test", sizeof(Razor::LoginRequest::password));

	client->emit(client->handle, packet);
}

void MultiplayerController::create_game()
{
	auto packet = Razor::Packet::create<Razor::CreateGame>();

	std::strncpy(packet.name, input_game_name, sizeof(Razor::CreateGame::name));
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
	auto packet = Razor::Packet::create<Razor::RefreshGamesList>();
	client->emit(client->handle, packet);
	refreshing_server_list = true;
}



void MultiplayerController::onLoginResponse(Razor::Packet* packet)
{
	std::cout << "ON_LOGIN_RESPONSE: " << packet->to_string() << std::endl;
}
