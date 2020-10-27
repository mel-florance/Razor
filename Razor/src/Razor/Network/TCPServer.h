#pragma once

#include "Razor/Application/Application.h"
#include "Network.h"

#define MAX_GENERATED_CLIENT_ID 100000
#define MAX_TOKEN_LENGTH 64

namespace Razor {

	struct GameInfo;

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
			uint32_t id;
			char name[20];
			char token[MAX_TOKEN_LENGTH];
		};

		struct Game {
			char name[20];
			unsigned int max_players;
			unsigned int players;
		};

		std::vector<uint32_t> generated_clients_ids;

		inline uint32_t generateClientId() 
		{
			srand(time(NULL));
			int n = rand() % MAX_GENERATED_CLIENT_ID;

			auto id_exists = [=](const uint32_t value) {
				auto it = std::find(generated_clients_ids.begin(), generated_clients_ids.end(), value);
				return it != generated_clients_ids.end();
			};

			while (id_exists(n)) {
				n = rand() % MAX_GENERATED_CLIENT_ID;
			}

			generated_clients_ids.push_back(n);

			return n;
		}

		std::unordered_map<SOCKET, Client> clients;
		std::unordered_map<uint32_t, std::vector<SOCKET>> channels;
		std::unordered_map<uint32_t, GameInfo> channels_infos;
	};

}
