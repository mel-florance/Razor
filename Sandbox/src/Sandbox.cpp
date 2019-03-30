#include <Razor.h>

class Sandbox : public Razor::Application
{
public:
	Sandbox()
	{
		PushOverlay(new Razor::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Razor::Application* Razor::createApplication()
{
	return new Sandbox();
}