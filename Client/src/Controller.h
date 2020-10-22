#pragma once

#include "Razor/Application/Application.h"
#include "Razor/Network/TCPClient.h"
#include "Interface.h"

class Controller
{
public:
	enum State {
		UNDEFINED,
		CONNECT,
		GAME,
		MAIN_MENU,
		GAMES_LIST,
		OPTIONS,
		LOBBY
	};

	Controller(std::shared_ptr<Razor::TCPClient> client, State state = State::UNDEFINED);
	~Controller();

	inline void setInterface(Interface* interface, const std::unordered_map<std::string, Razor::Texture*>& textures) {
		interface->controller = this;
		this->interface = interface;
		this->interface->textures = textures;
	}

	virtual void OnEvent(Razor::Event& event) {};
	virtual void OnUpdate(float delta) {};
	virtual void OnRender() {};

	State state;

protected:
	Interface* interface;
	std::shared_ptr<Razor::TCPClient> client;
};

