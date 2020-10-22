#include "Controller.h"

Controller::Controller(std::shared_ptr<Razor::TCPClient> client, State state) :
	client(client),
	state(state)
{
}

Controller::~Controller()
{
	delete interface;
}
