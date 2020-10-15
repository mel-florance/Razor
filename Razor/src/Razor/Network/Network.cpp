#include "rzpch.h"
#include "Network.h"

namespace Razor {

	const std::map<Network::State, std::pair<const char*, bool>> Network::States = {
		{ State::WINSOCK_INIT_FAILED, std::make_pair("Can't initialize winsock", false)},
		{ State::WINSOCK_INIT_SUCCEED, std::make_pair("Winsock initialized", true) },
		{ State::SOCKET_CREATE_FAILED, std::make_pair("Can't create socket", false) },
		{ State::SOCKET_CREATE_SUCCEED, std::make_pair("Socket created", true)},
		{ State::SOCKET_BIND_FAILED, std::make_pair("Can't bind the socket", false)},
		{ State::SOCKET_BIND_SUCCEED, std::make_pair("Socket binded", true) },
		{ State::SOCKET_LISTENING, std::make_pair("Socket listening", true) },
		{ State::SOCKET_CONNECTED, std::make_pair("New connection", true) },
		{ State::SOCKET_DISCONNECTED, std::make_pair("Client disconnected", false) },
		{ State::WELCOME_MESSAGE, std::make_pair("Welcome to the server !", true) },
	};

	void Network::log(Network::State code, bool error) {
		auto it = Network::States.find(code);;

		if (it != Network::States.end()) {
			if (!it->second.second)
				Log::error(it->second.first);
			else
				Log::info(it->second.first);
		}
	}

}