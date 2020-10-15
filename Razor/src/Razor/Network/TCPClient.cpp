#include "rzpch.h"
#include "TCPClient.h"
#include <WinSock2.h>
#include <Windows.h>

namespace Razor {

	std::vector<TCPClient::Message> TCPClient::Messages = {};

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

	bool TCPClient::Emit(const std::string message) {
		auto state = send(handle, message.c_str(), message.size() + 1, 0);

		if (state == SOCKET_ERROR) {
			std::cout << "Error at send(): " << WSAGetLastError() << std::endl;
		}

		return state != 0;
	}

	DWORD WINAPI TCPClient::ReadingThread(LPVOID param)
	{
		SOCKET s = (SOCKET)param;
		const int length = 4096;
		char buffer[length];
		int state;

		while (true) {
 			state = recv(s, buffer, length, 0);

			if (state <= 0) {
				Log::trace("Client disconnected");
				break;
			}

			buffer[state] = '\0';
			TCPClient::Messages.push_back({ buffer });
			Log::trace("Received: %s", buffer);
		}

		return 0;
	}

}
