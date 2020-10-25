#pragma once

#include "../Controller.h"
#include "../Interfaces/Home.h"

namespace Razor {
	class Texture;
}

class HomeController : public Controller
{
public:
	HomeController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~HomeController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};

