#include "rzpch.h"
#include "TCPServer.h"

namespace Razor {

	TCPServer::TCPServer()
	{
		init();
		auto sock = create_socket();
		bind_socket(sock);
		listen_socket(sock);
	}

	TCPServer::~TCPServer()
	{
	}

	void TCPServer::init()
	{
		WSADATA data;
		WORD version = MAKEWORD(2, 2);
		int state = WSAStartup(version, &data);

		if (state != 0)
			Network::log(Network::State::WINSOCK_INIT_FAILED);
		else
			Network::log(Network::State::WINSOCK_INIT_SUCCEED);
	}

	SOCKET TCPServer::create_socket()
	{
		SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

		if (listening == INVALID_SOCKET)
			Network::log(Network::State::SOCKET_CREATE_FAILED);
		else
			Network::log(Network::State::SOCKET_CREATE_SUCCEED);

		return listening;
	}

	void TCPServer::bind_socket(const SOCKET& sock)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(55555);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		int state = bind(sock, (sockaddr*)&hint, sizeof(hint));

		if (state == SOCKET_ERROR)
			Network::log(Network::State::SOCKET_BIND_FAILED);
		else
			Network::log(Network::State::SOCKET_BIND_SUCCEED);
	}

	void TCPServer::listen_socket(const SOCKET& listening)
	{
		int state = listen(listening, SOMAXCONN);
		Network::log(Network::State::SOCKET_LISTENING);

		fd_set master;
		FD_ZERO(&master);
		FD_SET(listening, &master);

		while (true) {
			fd_set copy = master;
			int size = select(0, &copy, nullptr, nullptr, nullptr);

			for (int i = 0; i < size; i++) {
				SOCKET sock = copy.fd_array[i];

				sockaddr_in addr;
				int addrlen = sizeof(addr);

				// Accept new connection
				if (sock == listening) {
					SOCKET client = accept(listening, (sockaddr*)&addr, &addrlen);
					FD_SET(client, &master);

					std::string connected = Network::getState(Network::State::SOCKET_CONNECTED);
					Log::info(std::string(connected + " (%s:%d)").c_str(), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

					std::string message = Network::getState(Network::State::WELCOME_MESSAGE);
					message += "\r\n";

					send(client, message.c_str(), message.size() + 1, 0);
				}
				// Accept new message
				else {
					const int length = 4096;
					char buffer[length];
					memset(buffer, 0, length);

					int bytes = recv(sock, buffer, length, 0);

					// Drop the connection
					if (bytes <= 0) {
						std::string disconnected = Network::getState(Network::State::SOCKET_DISCONNECTED);
						Log::info(disconnected.c_str());

						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else {
						// Broadcast the message
						Log::info(std::string("> " + std::string(buffer)).c_str());

						for (int i = 0; i < master.fd_count; i++) {
							SOCKET out = master.fd_array[i];

							if (out != listening && out != sock) {
								send(out, buffer, bytes, 0);
							}
						}
					}
				}
			}
		}

		WSACleanup();
	}
}