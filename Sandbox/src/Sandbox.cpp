#include <Razor.h>

class Sandbox : public Razor::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Razor::Application* Razor::createApplication()
{
	return new Sandbox();
}