#pragma once

#include "../Controller.h"
#include "../Interfaces/Loading.h"

namespace Razor {
	class Texture;
}

class LoadingController : public Controller
{
public:
	LoadingController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~LoadingController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	float progress;
};

