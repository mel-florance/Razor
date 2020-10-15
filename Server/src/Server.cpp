#include "Server.h"
#include <Razor/Network/TCPServer.h>

namespace Razor {

	Server::Server() : Application(true, Application::Type::SERVER)
	{
		auto instance = std::make_unique<TCPServer>();
	}

	Server::~Server()
	{
	}

}
