#pragma once

#include "Network.h"
#include "Packet.h"

namespace Razor {

	class TCPClient
	{
	public:
		TCPClient();
		~TCPClient();

		typedef std::function<void(Packet*)> PacketCallback;

		struct Message {
			std::string text;
			std::string time;
			std::string username;
			unsigned int id = 0;
		};

		bool Connect(const std::string& ip, int port);
		void Close();
		static DWORD WINAPI TCPClient::ReadingThread(LPVOID param);

		static void emit(SOCKET handle, const Packet& packet);
		static void bind(PacketType type, PacketCallback callback);

		SOCKET handle;
		clock_t timer_start;
		static unsigned int ping;
		static std::chrono::steady_clock::time_point ping_start;
		static std::vector<TCPClient::Message> Messages;
		static std::vector<std::string> Clients;
		static std::vector<GameInfo> Games;
		static uint32_t session_user_id;
		static char session_token[64];

		static std::unordered_map<PacketType, PacketCallback> bindings;
	};

}