#include "rzpch.h"

#include "TCPClient.h"
#include "Razor/Network/Packet.h"
#include "Razor/Core/Timer.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_internal.h"

namespace Razor {

	std::vector<TCPClient::Message> TCPClient::Messages = {};
	std::vector<std::string> TCPClient::Clients = {};
	std::vector<GameInfo> TCPClient::Games = {};
	std::chrono::steady_clock::time_point TCPClient::ping_start = {};
	unsigned int TCPClient::ping = 0;
	uint32_t TCPClient::session_user_id = 0;
	char TCPClient::session_token[64] = {};

	std::unordered_map<PacketType, TCPClient::PacketCallback> TCPClient::bindings = {};

	TCPClient::TCPClient()  
	{
		WSAData data;
		WORD version = MAKEWORD(2, 2);

		int state = WSAStartup(version, &data);

		if (state != 0) {
			Log::error("Cannot initialize winsock. Error: %d", WSAGetLastError());
			WSACleanup();
			return;
		}
		else {
			Log::trace("Winsock initialized");
		}
	}

	TCPClient::~TCPClient()
	{
		WSACleanup();
	}

	bool TCPClient::Connect(const std::string& ip, int port) {

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (handle == INVALID_SOCKET) {
			Log::error("Cannot create socket. Error: %d", WSAGetLastError());
			return false;
		}
		else {
			Log::trace("Socket created");
		}

		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

		int connection = connect(handle, (sockaddr*)&hint, sizeof(hint));

		if (connection == SOCKET_ERROR) {
			Log::error("Cannot connect to the server. Error: %d", WSAGetLastError());
			return false;
		}
		else {
			Log::trace("Connected to the server");
		}

		HANDLE thread;
		DWORD threadId;

		timer_start = clock();
		thread = CreateThread(NULL, 0, &ReadingThread, (void*)handle, 0, &threadId);

		if (!thread) {
			std::cout << "Error at CreateThread(): " << GetLastError() << std::endl;
			closesocket(handle);
			return false;
		}

		std::thread hearbeat_thread([=]() {
			while (true) {
				if ((clock() - timer_start) / CLOCKS_PER_SEC >= 2) {
					//std::cout << "Ping Heartbeat" << std::endl;

					timer_start = clock();
					ping_start = std::chrono::high_resolution_clock::now();

					int min = 27, max = 33;
					int range = max - min + 1;
					int ms = rand() % range + min;
					Sleep(ms);

					auto ping = Packet::create<Ping>();
					this->emit(this->handle, ping);
				}
			}
		});

		hearbeat_thread.detach();

		return true;
	}

	void TCPClient::Close() {
		closesocket(handle);
	}

	void TCPClient::emit(SOCKET sock, const Packet& packet) 
	{
		auto state = send(sock, (char*)&packet, packet.size, 0);

		if (state == SOCKET_ERROR) {
			std::cout << "Error at send(): " << WSAGetLastError() << std::endl;
		}
	}

	void TCPClient::bind(PacketType type, PacketCallback callback) {
		bindings[type] = callback;
	}

	DWORD WINAPI TCPClient::ReadingThread(LPVOID param)
	{
		SOCKET s = (SOCKET)param;
		const int length = 16384;
		char buffer[length];
		int state;

		while (true) {
		    state = recv(s, buffer, length, 0);

			if (state <= 0) {
				Log::trace("Client disconnected");
				break;
			}

			buffer[state] = '\0';

			tm* tm_local = Utils::getLocalTime();

			auto packet = reinterpret_cast<Packet*>(buffer);

			if (packet != nullptr) {

				for (auto it = bindings.begin(); it != bindings.end(); ++it) {
					if (it->first == packet->id) {
						it->second(packet);
					}
				}

				if (packet->id == PacketType::PONG) {
					auto end = std::chrono::high_resolution_clock::now();
					TCPClient::ping = std::chrono::duration_cast<std::chrono::milliseconds>(end - ping_start).count();
				}
				else if (packet->id == PacketType::GAME_INFO) {
				}
				else if (packet->id == PacketType::PLAYER_POSITION) {
				}
				else if (packet->id == PacketType::CLIENTS_LIST) {
					
				}
				else if (packet->id == PacketType::CLIENT_JOINED) {
					auto response = (ClientJoined*)packet;
					Message message;
					message.username = response->username;
					message.text = "has joined the server";
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					auto it = std::find(TCPClient::Clients.begin(), TCPClient::Clients.end(), response->username);

					if (it == TCPClient::Clients.end()) {
						TCPClient::Clients.push_back(response->username);
					}

					TCPClient::Messages.push_back(message);
				}
				else if (packet->id == PacketType::CLIENT_DISCONNECTED) {
					auto response = (ClientDisconnect*)packet;
					auto it = std::find(TCPClient::Clients.begin(), TCPClient::Clients.end(), response->username);

					if (it != TCPClient::Clients.end()) {
						TCPClient::Clients.erase(it);
					}

					Message message;
					message.username = response->username;
					message.text = "has disconnected";
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					TCPClient::Messages.push_back(message);

					std::cout << "User disconnected: " << response->username << std::endl;
				}
				else if (packet->id == PacketType::LOGIN_RESPONSE) {
					auto response = (LoginResponse*)packet;

					session_user_id = response->userId;

					Message message;
					message.text = response->message;
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					std::strncpy(session_token, response->token, sizeof(TCPClient::session_token));

					TCPClient::Messages.push_back(message);

					auto list = reinterpret_cast<ClientsList*>(&response->clients);

					std::cout << "CLIENTS LIST: " << std::endl;

					unsigned int i = 0;
					for (auto& c : list->clients) {
						auto info = reinterpret_cast<ClientInfo*>(&c);

						if (strlen(info->username) > 0) {
							TCPClient::Clients.push_back(info->username);

							std::cout << "USER INFO: " << std::string(info->username).c_str() << std::endl;
							++i;
						}
					}

					Log::trace("Received: %s", std::string(response->message).c_str());
				}
				//else if (packet->id == PacketType::GAME_CREATED) {
				//	auto game_infos = reinterpret_cast<GameCreated*>(packet);

				//	if (game_infos != nullptr) {

				//		auto infos = reinterpret_cast<GameInfo*>(&game_infos->infos);
				//		TCPClient::Games.push_back(*infos);

				//		if (session_user_id == infos->creator) {

				//		}

				//		std::cout << "Received new game infos:" << std::endl;
				//		std::cout << "------------------------" << std::endl;
				//		std::cout << "GameId: " << infos->gameId << std::endl;
				//		//std::cout << "Nb Players: " << sizeof(infos->players) << std::endl;
				//		std::cout << "Max Players: " << infos->max_players << std::endl;
				//		std::cout << "Ping: " << infos->ping << std::endl;
				//	}
				//}
				else if (packet->id == PacketType::GAMES_LIST) {
					auto list = reinterpret_cast<GamesList*>(packet);

					std::cout << "GAMES LIST: " << std::endl;

					for (auto game : list->games) {
						auto info = reinterpret_cast<GameInfo*>(&game);

						if (info != nullptr) {

							if (strlen(info->name) > 0) {
								std::cout << "INFO NAME: " << info->name << std::endl;

								auto it = std::find_if(TCPClient::Games.begin(), TCPClient::Games.end(),
								[=](const GameInfo& game_info) {
									return game_info.gameId == info->gameId;
								});

								if (it != TCPClient::Games.end()) {
									*it = *info;
								}
								else {
									TCPClient::Games.push_back(*info);
								}
							}
						}
					}
				}
				else if (packet->id == PacketType::GAME_DESTROYED) {
					auto game_infos = reinterpret_cast<GameDestroyed*>(packet);
					std::cout << "RECEIVE GAME DESTROY" << std::endl;

					if (game_infos != nullptr) {
						std::vector<GameInfo>::iterator it = Games.begin();

						for (; it != Games.end();) {
							if (it->gameId == game_infos->gameId) {
								it = Games.erase(it);
								Log::trace("Game %d destroyed", game_infos->gameId);
							}
							else {
								it++;
							}
						}
					}
				}
			}

			memset(buffer, 0, 4096);
		}

		return 0;
	}

}
