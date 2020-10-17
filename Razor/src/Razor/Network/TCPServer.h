#pragma once

#include "Razor/Application/Application.h"
#include "Network.h"

namespace Razor {

	class TCPServer
	{
	public:
		TCPServer();
		~TCPServer();

		void init();
		SOCKET create_socket();
		void bind_socket(const SOCKET& sock);
		void listen_socket(const SOCKET& sock);

	private:
		struct Client {
			char name[20];
		};

		std::unordered_map<SOCKET, Client> clients;
	};

}
