#include <Razor.h>
#include "Server.h"

Razor::Application* Razor::createApplication()
{
	return new Razor::Server();
}
