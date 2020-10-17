#include "rzpch.h"

#include "TCPClient.h"
#include "Razor/Network/Packet.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_internal.h"

namespace Razor {

	std::vector<TCPClient::Message> TCPClient::Messages = {};
	std::vector<std::string> TCPClient::Clients = {};

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

		thread = CreateThread(NULL, 0, &ReadingThread, (void*)handle, 0, &threadId);

		if (!thread) {
			std::cout << "Error at CreateThread(): " << GetLastError() << std::endl;
			closesocket(handle);
			return false;
		}

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

	DWORD WINAPI TCPClient::ReadingThread(LPVOID param)
	{
		SOCKET s = (SOCKET)param;
		const int length = 8192;
		char buffer[length];
		int state;

		while (true) {
 			state = recv(s, buffer, length, 0);

			if (state <= 0) {
				Log::trace("Client disconnected");
				break;
			}

			buffer[state] = '\0';

			time_t curr_time;
			curr_time = time(NULL);
			tm* tm_local = localtime(&curr_time);

			auto packet = reinterpret_cast<Packet*>(buffer);

			if (packet != nullptr) {
				if (packet->id == PacketType::PONG) {
				}
				else if (packet->id == PacketType::GAMES_LIST) {
				}
				else if (packet->id == PacketType::GAME_INFO) {
				}
				else if (packet->id == PacketType::PLAYER_POSITION) {
				}
				else if (packet->id == PacketType::CLIENTS_LIST) {
					
				}
				else if (packet->id == PacketType::CLIENT_JOINED) {
					auto response = (PacketClientJoined*)packet;
					Message message;
					message.username = response->username;
					message.text = "has joined the server";
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					TCPClient::Messages.push_back(message);
				}
				else if (packet->id == PacketType::CLIENT_DISCONNECTED) {
					auto response = (PacketClientDisconnect*)packet;
				}
				else if (packet->id == PacketType::CHAT_MESSAGE) {
					auto item = (PacketChatMessage*)packet;

					Message message;
					message.username = item->username;
					message.text = item->message;
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					TCPClient::Messages.push_back(message);

					Log::trace("Received: %s", std::string(item->message).c_str());
				}
				else if (packet->id == PacketType::LOGIN_RESPONSE) {
					auto response = (PacketLoginResponse*)packet;

					Message message;
					message.text = response->message;
					message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

					std::cout << response->token << std::endl;
					TCPClient::Messages.push_back(message);

					auto list = reinterpret_cast<PacketClientsList*>(&response->clients);

					std::cout << "CLIENTS LIST: " << std::endl;

					unsigned int i = 0;
					for (auto& c : list->clients) {
						auto info = reinterpret_cast<PacketClientInfo*>(&c);

						if (strlen(info->username) > 0) {
							TCPClient::Clients.push_back(info->username);
							std::cout << "USER INFO: " << std::string(info->username).c_str() << std::endl;
							++i;
						}
					}

					Log::trace("Received: %s", std::string(response->message).c_str());
				}
			}

			memset(buffer, 0, 4096);
		}

		return 0;
	}

}
