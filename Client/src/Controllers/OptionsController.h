#pragma once

#include "../Controller.h"
#include "../Interfaces/Options.h"

namespace Razor {
	class Texture;
}

class OptionsController : public Controller
{
public:
	OptionsController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~OptionsController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;
};

