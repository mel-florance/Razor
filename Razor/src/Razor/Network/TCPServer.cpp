#include "rzpch.h"
#include "TCPServer.h"

#include "Razor/Network/Packet.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>.

#include "Razor/Maths/sha512.h"

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

			for (int i = 0; i < size; i++) 
			{
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
					const size_t length = 4096;
					char buffer[length];
					memset(buffer, 0, length);

					int bytes = recv(sock, buffer, length, 0);
			
					// Drop the connection
					if (bytes <= 0) {
						std::string disconnected = Network::getState(Network::State::SOCKET_DISCONNECTED);
						Log::info(disconnected.c_str());

						auto it = clients.find(sock);

						if (it != clients.end());
							clients.erase(it);

						std::unordered_map<SOCKET, Client>::iterator i = clients.begin();

						for (; i != clients.end(); ++i)
							std::cout << "Client: " << (*i).second.name << std::endl;

						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else {
						auto packet = reinterpret_cast<Packet*>(buffer);

						if (packet != nullptr) {
							std::cout << "Packet type: " << Protocol::to_string(packet->id) << std::endl;

							if (packet->id == PacketType::LOGIN) {
								auto login = reinterpret_cast<PacketLoginRequest*>(packet);

								for (int i = 0; i < master.fd_count; i++) {
									SOCKET out = master.fd_array[i];

									if (out != listening && out == sock) {
										auto response = Packet::create<PacketLoginResponse>();
										response.status = RequestStatus::LOGGED;

										auto message = std::string("Welcome to the server ") + login->username;
										std::strncpy(response.message, message.data(), sizeof(PacketLoginResponse::message));

										auto token = Utils::random_string(64);
										std::strncpy(response.token, token.data(), sizeof(PacketLoginResponse::token));

										Client client;
										std::strncpy(client.name, login->username, sizeof(Client::name));

										clients.insert(std::make_pair(sock, client));

										unsigned int i = 0;
										std::unordered_map<SOCKET, Client>::iterator it = clients.begin();
										auto pkt = Packet::create<PacketClientsList>();

										for (; it != clients.end(); ++it, ++i) {
											std::cout << "Client: " << (*it).second.name << std::endl;

											auto info = Packet::create<PacketClientInfo>();
											std::strncpy(info.username, (*it).second.name, sizeof(PacketClientInfo::username));
											pkt.clients[i] = info;
										}

										response.clients = pkt;
										send(out, reinterpret_cast<char*>(&response), sizeof(PacketLoginResponse), 0);
									}
									else if (out != listening && out != sock) {
										auto response = Packet::create<PacketClientJoined>();

										std::strncpy(response.username, login->username, sizeof(PacketClientJoined::username));
										send(out, reinterpret_cast<char*>(&response), sizeof(PacketClientJoined), 0);
									}
								}
							} 
							else if (packet->id == PacketType::GAMES_LIST) {
								auto list = reinterpret_cast<PacketGamesList*>(packet);
								
								std::cout << "Game infos: " << std::endl;
								std::cout << "---------------------------" << std::endl;

								for (auto& item : list->games) {
									auto game = reinterpret_cast<PacketGameInfo*>(&item);

									if (game->gameId != 0) {
										std::cout << "id: " << game->gameId << std::endl;
										std::cout << "ping: " << game->ping << std::endl;
										std::cout << "players: " << game->players << std::endl;
										std::cout << "---------------------------" << std::endl;
									}
								}
							}
							else if (packet->id == PacketType::CHAT_MESSAGE) {
								auto chatMessage = reinterpret_cast<PacketChatMessage*>(packet);

								if (chatMessage != nullptr) {
									std::cout << "Message: " << chatMessage->message << std::endl;
									std::strncpy(chatMessage->username, clients[sock].name, sizeof(PacketChatMessage::username));

									// Broadcast the message
									Log::info(std::string("> " + std::string(buffer)).c_str());

									for (int i = 0; i < master.fd_count; i++) {
										SOCKET out = master.fd_array[i];
										send(out, buffer, bytes, 0);
									}
								}
							}
							else if (packet->id == PacketType::PLAYER_POSITION) {
								auto player = reinterpret_cast<PacketPlayerPosition*>(packet);

								if (player != nullptr) {
									std::cout << "Player Position: " << player->position[0] << ", "
																	 << player->position[1] << ", "
																	 << player->position[2] << std::endl;
								}
							}
						}
					}
				}
			}
		}

		WSACleanup();
	}
}