#include "rzpch.h"
#include "Http.h"

namespace Razor
{

	Http::Http()
	{
	}

	Http::~Http()
	{
	}

	std::string Http::get(const std::string& url, const std::string& endpoint)
	{
		unsigned short port = 80;
		WSADATA data;
		char buffer[10000];
		SOCKET sock;
		SOCKADDR_IN sock_addr;
		struct hostent* host;
		std::string html;
		std::string formatted_url = formatRequestHeader(url, endpoint, RequestType::GET);
		ADDRINFOW addr_info_hints;
		PADDRINFOW addr_info_list;

		memset(&addr_info_hints, 0, sizeof(ADDRINFOW));
		memset(&addr_info_list, 0, sizeof(PADDRINFOW));

		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			Log::error("HTTP Request: Can't init winsock, wsa startup failed.");
			return std::string();
		}

		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		host = gethostbyname(url.c_str());
		
		const size_t cSize = strlen(url.c_str()) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, url.c_str(), cSize);

		const size_t pSize = strlen(std::to_string(port).c_str()) + 1;
		wchar_t* wcp = new wchar_t[pSize];
		mbstowcs(wcp, std::to_string(port).c_str(), pSize);

		GetAddrInfo(wc, wcp, &addr_info_hints, &addr_info_list);
		
		sock_addr.sin_port = htons(80);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		if (connect(sock, (SOCKADDR*)(&sock_addr), sizeof(sock_addr)) != 0)
		{
			Log::error("HTTP: Can't connect to url %s", url);
			return std::string();
		}

		send(sock, formatted_url.c_str(), (int)strlen(formatted_url.c_str()), 0);

		int length;

		while ((length = recv(sock, buffer, 10000, 0)) > 0)
		{
			int i = 0;

			while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
			{
				html += buffer[i];
				i += 1;
			}
		}

		closesocket(sock);
		WSACleanup();

		return html;
	}

	std::string Http::requestTypeToStr(RequestType type)
	{
		switch (type) {
		default:
		case RequestType::GET: return "GET";
		case RequestType::POST: return "POST";
		}
		
		return "Unknown";
	}

	std::string Http::formatRequestHeader(const std::string& url, const std::string& endpoint, RequestType type)
	{
		return requestTypeToStr(type) + " " + endpoint + " HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";
	}
}
