#pragma once

#include <WS2tcpip.h>
#pragma comment (lib, "WS2_32.lib")

#include "Http.h"

namespace Razor {

	class Network {
	public:
		enum class State {
			WINSOCK_INIT_FAILED,
			WINSOCK_INIT_SUCCEED,
			SOCKET_CREATE_FAILED,
			SOCKET_CREATE_SUCCEED,
			SOCKET_BIND_FAILED,
			SOCKET_BIND_SUCCEED,
			SOCKET_LISTENING,
			SOCKET_CONNECTED,
			SOCKET_DISCONNECTED,
			WELCOME_MESSAGE,
		};

		static const std::map<State, std::pair<const char*, bool>> States;
		static void log(State code, bool error = false);

		static inline const char* getState(State state) {
			const char* out = "";
			auto it = States.find(state);

			if (it != States.end())
				out = it->second.first;

			return out;
		}
	};

}
