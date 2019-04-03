#include "Server.h"

namespace Razor { namespace Network {

	Server::Server() : Application(true)
	{
		std::cout << "Server started !" << std::endl;

		system("pause");
	}

	Server::~Server()
	{
	}

	bool Server::start(unsigned short port)
	{
		return false;
	}

	void Server::stop()
	{
	}

	void Server::update()
	{
	}

	bool Server::emit(uint64_t clientid, const unsigned char* data, unsigned int len)
	{
		return false;
	}

	bool Server::broadcast(const unsigned char* data, unsigned int len)
	{
		return false;
	}

}}