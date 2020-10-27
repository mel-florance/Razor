#pragma once

#include "../Controller.h"
#include "../Interfaces/Game.h"

namespace Razor {
	class Texture;
}

class GameController : public Controller
{
public:
	GameController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~GameController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};

