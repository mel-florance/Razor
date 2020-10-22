#pragma once

#include "../Controller.h"
#include "../Interfaces/Lobby.h"

namespace Razor {
	class Packet;
	class Texture;
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

	static void onPlayerReady(Razor::Packet* packet);
	static void onGameCreated(Razor::Packet* packet);

	static Razor::GameInfo current_game_infos;
};

