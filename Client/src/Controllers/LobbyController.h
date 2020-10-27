#pragma once

#include "../Controller.h"
#include "../Interfaces/Lobby.h"

namespace Razor {
	class Texture;
	class Clock;
}

class LobbyController : public Controller
{
public:
	LobbyController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~LobbyController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	void set_player_ready();
	void leave_lobby();

	static void onPlayerReady(Razor::Packet* packet);
	static void onGameCreated(Razor::Packet* packet);
	static void onPlayerStrangerJoined(Razor::Packet* packet);
	static void onPlayerSelfJoined(Razor::Packet* packet);
	static void onPlayerLeaved(Razor::Packet* packet);
	static void onMatchReady(Razor::Packet* packet);

	static Razor::GameInfo current_game_infos;
	static bool is_player_ready;
	static bool match_started;
	static float match_countdown_total;
	static float match_countdown_elapsed;
	static std::unique_ptr<Razor::Clock> match_countdown;
	static unsigned int countdown_print_status;
};

