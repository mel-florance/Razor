#pragma comment (lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <Razor.h>
#include "Server.h"

Razor::Application* Razor::createApplication()
{
	return new Razor::Network::Server();
}
