#include <Razor.h>
#include "Client.h"

Razor::Application* Razor::createApplication()
{
	return new Razor::Client();
}
