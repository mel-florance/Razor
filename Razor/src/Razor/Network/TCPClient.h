#pragma once

#include "Network.h"
#include "Packet.h"

namespace Razor {

	class TCPClient
	{
	public:
		TCPClient();
		~TCPClient();

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

		SOCKET handle;
		static std::vector<TCPClient::Message> Messages;
		static std::vector<std::string> Clients;
		char session_token[64];
		
	};

}