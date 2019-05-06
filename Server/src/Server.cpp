#include "Server.h"

namespace Razor { namespace Network {

	Server::Server() : Application(true)
	{
		std::cout << "Server started !" << std::endl;

		Variant v("hehe");
		Variant n(3);
		Array<Variant> arr({ {false}, n, {"test"}, {8}, { 1.2f } });

		arr += v;
		arr -= n;

		for (auto& item : arr) {
			Log::info("value: %s", item.toString().c_str());
		}

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