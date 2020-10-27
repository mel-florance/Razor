#include "rzpch.h"
#include "TCPServer.h"

#include "Razor/Network/Packet.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Razor/Maths/sha512.h"

namespace Razor {

	TCPServer::TCPServer() :
		clients({}),
		channels({}),
		channels_infos({}),
		generated_clients_ids({})
	{
		init();
		auto sock = create_socket();
		bind_socket(sock);
		listen_socket(sock);

		srand(time(NULL));
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

				sockaddr_in addr = { 0 };
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
						getpeername(sock, (sockaddr*)&addr, &addrlen);

						std::string disconnected = Network::getState(Network::State::SOCKET_DISCONNECTED);
						Log::info(std::string(disconnected + " (%s:%d)").c_str(), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

						//auto clients_it = clients.find(sock);

						//if (clients_it != clients.end());
						//	clients.erase(clients_it);

						std::unordered_map<uint32_t, std::vector<SOCKET>>::iterator ch = channels.begin();

						for (; ch != channels.end(); ++ch) {
							std::vector<SOCKET>::iterator it2 = ch->second.begin();

							for (; it2 != ch->second.end(); ++it2) {
								if (*it2 == sock) {
									ch->second.erase(it2);
									break;
								}
							}
						}

						std::unordered_map<uint32_t, std::vector<SOCKET>>::iterator ch2 = channels.begin();
						std::vector<uint32_t> to_erase = {};

						for (; ch2 != channels.end(); ++ch2) {
							if (ch2->second.size() == 0) {
								to_erase.push_back(ch2->first);
							}
						}

						if (to_erase.size() > 0) {
							for (auto id : to_erase) {
								auto channel_it = channels.find(id);

								if (channel_it != channels.end()) {
									channels.erase(channel_it);
								}

								auto info_it = channels_infos.find(id);

								if (info_it != channels_infos.end())
									channels_infos.erase(info_it);
							}
						}

						for (int i = 0; i < master.fd_count; i++) {
							SOCKET out = master.fd_array[i];

							if (out != listening && out != sock) {
								if (to_erase.size() > 0) {
									auto response = Packet::create<GameDestroyed>();
									response.gameId = to_erase[0];
									send(out, reinterpret_cast<char*>(&response), sizeof(GameDestroyed), 0);
								}

								auto response = Packet::create<ClientDisconnect>();

								std::strncpy(response.username, clients[sock].name, sizeof(ClientDisconnect::username));
								send(out, reinterpret_cast<char*>(&response), sizeof(ClientDisconnect), 0);
							}
						}

						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else {
						auto packet = reinterpret_cast<Packet*>(buffer);
		
						if (packet != nullptr) {
							bool authenticated = false;

							for (auto it = clients.begin(); it != clients.end(); ++it) { 
								if (it->second.token == packet->token) {
									authenticated = true;
								}
							}

							if (packet->id != PacketType::PING) {
								getpeername(sock, (sockaddr*)&addr, &addrlen);

								Log::warn(
									"[IN] %s - %s (%s:%d)", 
									packet->to_string().c_str(),
									Utils::bytesToSize(bytes).c_str(),
									inet_ntoa(addr.sin_addr),
									ntohs(addr.sin_port)
								);
							}

							if (packet->id == PacketType::LOGIN) {
								auto login = reinterpret_cast<LoginRequest*>(packet);

								for (int i = 0; i < master.fd_count; i++) {
									SOCKET out = master.fd_array[i];

									if (out != listening && out == sock) {
										auto response = Packet::create<LoginResponse>();
										response.status = Protocol::RequestStatus::LOGGED;

										auto token = Utils::random_string(64);
										std::strncpy(response.token, token.data(), sizeof(LoginResponse::token));

										Client client;
										client.id = generateClientId();
										std::strncpy(client.name, login->username, sizeof(Client::name));
										std::strncpy(client.token, token.data(), sizeof(Client::token));

										clients.insert(std::make_pair(sock, client));

										response.userId = clients[sock].id;
										send(out, reinterpret_cast<char*>(&response), sizeof(LoginResponse), 0);
									}
								}
							} 
							else if (packet->id == PacketType::PING) {
								auto response = Packet::create<Pong>();
								send(sock, reinterpret_cast<char*>(&response), sizeof(Pong), 0);
							}
							else if (packet->id == PacketType::CHAT_MESSAGE) {
								auto chatMessage = reinterpret_cast<ChatMessage*>(packet);

								if (chatMessage != nullptr) {
									std::strncpy(chatMessage->username, clients[sock].name, sizeof(ChatMessage::username));

									for (int i = 0; i < master.fd_count; i++) {
										SOCKET out = master.fd_array[i];
										send(out, buffer, bytes, 0);
									}
								}
							}
							else if (packet->id == PacketType::PLAYER_POSITION) {
								auto player = reinterpret_cast<PlayerPosition*>(packet);

								if (player != nullptr) {
									std::cout << "Player Position: " << player->position[0] << ", "
																	 << player->position[1] << ", "
																	 << player->position[2] << std::endl;
								}
							}
							else if (packet->id == PacketType::CREATE_GAME) {
								auto params = reinterpret_cast<CreateGame*>(packet);

								if (params != nullptr) {
									std::unordered_map<uint32_t, std::vector<SOCKET>>::iterator it = channels.begin();

									unsigned int count = 0;

									for (; it != channels.end(); ++it) {
										std::vector<SOCKET>::iterator it2 = it->second.begin();

										for (; it2 != it->second.end(); ++it2) {
											if (*it2 == sock) {
												count++;
												break;
											}
										}
									}

									if (count == 0) {
										uint32_t channel = channels.size();

										std::vector<SOCKET> array;
										array.push_back(sock);
										channels[channel] = array;

										auto response = Packet::create<GameCreated>();
										auto info = Packet::create<GameInfo>();
										auto creator = clients.find(sock);

										if (creator != clients.end()) {

											info.creator = creator->second.id;
											info.gameId = channel;
											info.ping = rand() % 100 + 1;
											info.max_players = params->max_players;
											info.players_count = 1;
											info.mode = params->mode;
											info.map = params->map;
											std::strncpy(info.name, params->name, sizeof(GameInfo::name));

											auto players = Packet::create<PlayersList>();
											auto player = Packet::create<PlayerInfo>();
											player.ready = false;
											player.userId = clients[sock].id;
											std::strncpy(player.username, clients[sock].name, sizeof(PlayerInfo::username));

											players.players[0] = player;
											info.players = players;

											channels_infos[channel] = info;
											response.infos = info;

											for (int i = 0; i < master.fd_count; i++) {
												SOCKET out = master.fd_array[i];
												send(out, reinterpret_cast<char*>(&response), sizeof(GameCreated), 0);
											}

											Log::info("Game created with id %d", channel);
										}
									}
								}
							}
							else if (packet->id == PacketType::LEAVE_GAME) {
								auto request = reinterpret_cast<LeaveGame*>(packet);
								auto it = channels_infos.begin();

								std::cout << "Game leave requested:" << request->gameId << std::endl;

								for (; it != channels_infos.end(); ++it) {
									if (it->second.gameId == request->gameId) {
										auto& list = channels[it->first];
										list.erase(std::remove(list.begin(), list.end(), sock), list.end());
										it->second.players_count--;

										PlayerInfo info;
										info.userId = clients[sock].id;
										std::strncpy(info.username, clients[sock].name, sizeof(PlayerInfo::username));

										for (auto& p : it->second.players.players) {
											if (p.userId == info.userId) {
												p.userId = 0;
												p.ready = false;
												std::strncpy(p.username, "", sizeof(PlayerInfo::username));
											}
										}

										std::cout << "player has leaved" << std::endl;

										for (auto s : list) {
											auto response = Packet::create<PlayerLeaved>();
											response.info = info;
											send(s, reinterpret_cast<char*>(&response), sizeof(PlayerLeaved), 0);
										}

										break;
									}
								}
							}
							else if (packet->id == PacketType::JOIN_GAME) {
								auto request = reinterpret_cast<JoinGame*>(packet);
								auto it = channels_infos.begin();
			
								std::cout << "Game join requested:" << request->gameId << std::endl;

								for (; it != channels_infos.end(); ++it) {
									if (it->second.gameId == request->gameId) {
										std::cout << "Game found!" << std::endl;
										size_t count = 0;

										for (auto& p : it->second.players.players)
											if (strlen(p.username) > 0)
												count++;

										if (count + 1 > it->second.max_players)
											break;

										bool found = false;
										for (auto& player : it->second.players.players) {
											if (player.userId == clients[sock].id) {
												found = true;
											}
										}

										std::cout << "Player count:" << count << std::endl;
										std::cout << "Exists:" << (found ? "yes" : "no") <<  std::endl;

										if (!found) {
											size_t len = sizeof(it->second.players.players) / sizeof(it->second.players.players[0]);

											for (unsigned int i = 0; i < len; ++i) {
												auto& player = it->second.players.players[i];

												if (strlen(player.username) == 0) {
													std::cout << "Slot found" << std::endl;
													channels[it->first].push_back(sock);

													PlayerInfo infos;
													infos.userId = clients[sock].id;
													std::strncpy(infos.username, clients[sock].name, sizeof(PlayerInfo::username));
													it->second.players.players[i] = infos;
													it->second.players_count++;

													std::cout << "PLAYER COUNT: " << it->second.players_count << std::endl;

													auto chan_it = channels.begin();

													std::cout << "CHANNEL SIZE: " << channels[it->first].size() << std::endl;

													for (auto s : channels[it->first]) {
														if (s == sock) {
															auto response = Packet::create<PlayerSelfJoined>();
															response.info = channels_infos[it->first];
															send(s, reinterpret_cast<char*>(&response), sizeof(PlayerSelfJoined), 0);
														}
														else {
															auto response = Packet::create<PlayerStrangerJoined>();
															response.info = channels_infos[it->first];
															response.userId = infos.userId;
															send(s, reinterpret_cast<char*>(&response), sizeof(PlayerStrangerJoined), 0);
														}
													}

													break;
												}
											}
										}

										break;
									}
								}
							}
							else if (packet->id == PacketType::MARK_PLAYER_READY) {
								auto request = reinterpret_cast<MarkPlayerReady*>(packet);

								if (request != nullptr) {
									auto it = channels_infos.begin();

									for (; it != channels_infos.end(); ++it) {
										if (it->second.gameId == request->gameId) {
											for (auto& player : it->second.players.players) {
												if (player.userId == clients[sock].id) {
													player.ready = request->ready;

													auto response = Packet::create<PlayerReady>();
													response.userId = player.userId;
													response.ready = player.ready;

													for (auto s : channels[it->first]) {
														send(s, reinterpret_cast<char*>(&response), sizeof(PlayerReady), 0);
													}

													break;
												}
											}

											int ready = 0;
											for (auto& player : it->second.players.players)
												if (player.ready)
													ready++;

											if (ready == it->second.players_count) {
												auto response = Packet::create<MatchReady>();

												for (auto s : channels[it->first]) {
													send(s, reinterpret_cast<char*>(&response), sizeof(MatchReady), 0);
												}
											}
										}
									}
								}
							}
							else if (packet->id == PacketType::REFRESH_GAMES_LIST)
							{
								auto response = Packet::create<GamesList>();
								auto it = channels.begin();
								unsigned int j = 0;

								for (; it != channels.end(); ++it, j++) {
									auto info = Packet::create<GameInfo>();
									auto index = channels_infos.find(it->first);

									if (index != channels_infos.end()) {
										std::strncpy(info.name, index->second.name, sizeof(GameInfo::name));
										info.gameId = index->second.gameId;
										info.ping = index->second.ping;
										info.mode = index->second.mode;
										info.map = index->second.map;
									
										
										int count = 0;

										for (unsigned int i = 0; i < MAX_PLAYERS_LIST; i++) {
											if (strlen(index->second.players.players[i].username) > 0) {
												count++;
											}
										}
										
										info.players_count = count;
										info.max_players = index->second.max_players;

										response.games[j] = info;
									}
								}

								send(sock, reinterpret_cast<char*>(&response), sizeof(GamesList), 0);
							}
						}
					}
				}
			}
		}

		WSACleanup();
	}
}
