#include <Razor.h>

class TestLayer : public Razor::Layer
{
public:
	TestLayer() : Layer("Test")
	{
	}
	
};

class Sandbox : public Razor::Application
{		
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox()
	{

	}
};

Razor::Application* Razor::createApplication()
{
	RZ_INFO("Started Application");

	return new Sandbox();
}