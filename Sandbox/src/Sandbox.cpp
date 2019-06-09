#include <Razor.h>
//#include <vld.h>

class TestLayer : public Razor::Layer
{
public:
	TestLayer(Razor::ScenesManager* sceneManager) : Layer("Test"), sm(sceneManager)
	{
		
	}

	void OnAttach()
	{
		Razor::Script script;
	}

	void createPlane() 
	{
		std::shared_ptr<Razor::Node> node = std::make_shared<Razor::Node>();
		node->name = "Plane";
		node->transform.setScale(glm::vec3(8.0f, 1.0f, 8.0f));
		std::shared_ptr<Razor::PhongMaterial> mat = std::make_shared<Razor::PhongMaterial>();
		std::shared_ptr<Razor::Plane> plane = std::make_shared<Razor::Plane>();
		plane->setMaterial(mat);

		node->meshes.push_back(plane);
		sm->getActiveScene()->getSceneGraph()->addNode(node);
	}

	void OnEvent(Razor::Event& event) 
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if(e.GetKeyCode() == RZ_KEY_J)
				createPlane();
		}
	}

private:
	Razor::ScenesManager* sm;
};

class Sandbox : public Razor::Application
{		
public:
	Sandbox()
	{
		Razor::ScenesManager* sm = this->getScenesManager();
		PushLayer(new TestLayer(sm));
	}

	~Sandbox()
	{

	}
};

Razor::Application* Razor::createApplication()
{
	return new Sandbox();
}