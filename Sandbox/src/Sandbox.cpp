#include <Razor.h>

class TestLayer : public Razor::Layer
{
public:
	TestLayer(Razor::ScenesManager* sceneManager) : Layer("Test"), sm(sceneManager)
	{
		
	}

	void OnAttach() 
	{
		Razor::Node* node = new Razor::Node();
		node->transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		node->transform.setScale(glm::vec3(8.0f, 1.0f, 8.0f));
		Razor::PhongMaterial* mat = new Razor::PhongMaterial();
		mat->setDiffuseColor(glm::vec3(0.8f, 0.8f, 0.8f));
		Razor::Plane* plane = new Razor::Plane();
		plane->setMaterial(mat);
		node->meshes.push_back(plane);
		//sm->getActiveScene()->getSceneGraph()->addNode(node);

		Razor::Script script;

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
	//Log::info("Started Application");

	return new Sandbox();
}