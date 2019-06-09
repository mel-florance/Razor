#pragma once

#include "Razor/Application/Application.h"

#include "Client.h"

namespace Razor { namespace Network {

	class Server : public Razor::Application
	{
	public:
		Server();
		~Server();

		bool start(unsigned short port);
		void stop();
		void update();
		bool emit(uint64_t clientid, const unsigned char* data, unsigned int len);
		bool broadcast(const unsigned char* data, unsigned int len);

	private:
		std::map<uint64_t, Client> clients;
	};

}}
