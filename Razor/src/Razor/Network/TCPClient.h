#pragma once

#include "Network.h"

namespace Razor {

	class TCPClient
	{
	public:
		TCPClient();
		~TCPClient();

		struct Message {
			std::string text;
			unsigned int id = 0;
			unsigned int time = 0;
		};

		bool Connect(const std::string& ip, int port);
		void Close();
		bool Emit(const std::string message);
		static DWORD WINAPI TCPClient::ReadingThread(LPVOID param);

		SOCKET handle;
		static std::vector<TCPClient::Message> Messages;
	};

}