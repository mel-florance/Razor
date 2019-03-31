#include <Razor.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>


#include "imgui/imgui.h"

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	return Projection * View * Model;
}

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
	RZ_FILE_INFO("Started Application");

	return new Sandbox();
}