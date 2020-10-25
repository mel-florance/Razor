#pragma once

#include "../Controller.h"
#include "../Interfaces/Multiplayer.h"

namespace Razor {
	struct GameInfo;
}

enum GameMode : uint32_t {
	CASUAL,
	COMPETITIVE,
	DEATHMATCH,
	ARMS_RACE,
	BATTLE_ROYALE
};

class MultiplayerController : public Controller
{
public:
	MultiplayerController(
		std::shared_ptr<Razor::TCPClient> client, 
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~MultiplayerController();

	inline static std::string game_mode_str(int mode) {
		switch (mode) {
		default:
		case GameMode::CASUAL: return "Casual";
		case GameMode::COMPETITIVE: return "Competitive";
		case GameMode::DEATHMATCH: return "Deathmatch";
		case GameMode::ARMS_RACE: return "Arms race";
		case GameMode::BATTLE_ROYALE: return "Battle Royale";
		}
	}

	inline static const std::array<const char*, 5> game_modes_str() {
		return {
			"Casual",
			"Competitive",
			"Deathmatch",
			"Arms race",
			"Battle Royale"
		};
	}

	const static std::unordered_map<uint32_t, std::string> maps;

	inline static const std::vector<const char*> maps_str() {
		std::vector<const char*> out;

		for (auto it = maps.begin(); it != maps.end(); ++it) {
			out.push_back(it->second.c_str());
		}

		return out;
	}

	inline static std::string get_map_name(uint32_t map) {
		auto it = maps.find(map);

		if (it != maps.end()) {
			return it->second;
		}

		return std::to_string(map);
	}

	inline static uint32_t get_map_from_name(const char* name) {
		for (auto it = maps.begin(); it != maps.end(); ++it) {
			if (it->second == name) {
				return it->first;
			}
		}

		return 0;
	}

	inline static uint32_t get_mode_from_name(const char* name) {
		auto names = game_modes_str();

		for (unsigned int i = 0; i < 5; ++i) {
			if (names.at(i) == name) {
				return i;
			}
		}

		return 0;
	}

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	void login();
	void create_game();
	void refresh_games();
	void join_game(uint32_t gameId);


	static void onLoginResponse(Razor::Packet* packet);
	static void onGameDestroyed(Razor::Packet* packet);
	static void onGamesList(Razor::Packet* packet);

	static std::vector<Razor::GameInfo> games;

	char input_name[20];
	char input_game_name[20];
	GameMode input_game_mode;
	const char* current_game_mode;
	uint32_t input_game_map;
	const char* current_game_map;
	int input_game_max_players;
	bool refreshing_server_list;

	static uint32_t session_user_id;
	static char session_token[64];

	int port;
	std::string host;
	static bool connected;
};

