#include <Razor.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		auto cam = camera(5.0f, { 0.5f, 0.5f });
		std::cout << glm::to_string(cam) << std::endl;

		
	}

	void OnUpdate() override
	{
		if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		{
			RZ_INFO("Tab key is pressed!(poll)");
		}
	}

	void OnEvent(Razor::Event& event) override
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_TAB)
			{
				
			}

			RZ_INFO("{0}", (char)e.GetKeyCode());
		}
	}

	void OnImGuiRender() override
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);


		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_DockNodeHost;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();


		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		//ImGui::SetWindowFontScale(1.1f);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl + N", false, true);
				ImGui::MenuItem("Open", "Ctrl + O", false, true);
				ImGui::MenuItem("Open Recent...", "Ctrl + Shift + O", false, true);
				ImGui::Separator();
				ImGui::MenuItem("Save", "Ctrl + S", false, true);
				ImGui::MenuItem("Save As...", "Ctrl + Shift + S", false, true);
				ImGui::Separator();
				ImGui::MenuItem("Preferences", NULL, false, true);
				ImGui::Separator();
				ImGui::MenuItem("Import...", NULL, false, true);
				ImGui::MenuItem("Export...", NULL, false, true);
				ImGui::Separator();
				ImGui::MenuItem("Quit", "Ctrl + Q", false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Cut", "Ctrl + X", false, true);
				ImGui::MenuItem("Copy", "Ctrl + C", false, true);
				ImGui::MenuItem("Paste", "Ctrl + V", false, true);
				ImGui::MenuItem("Delete", "Del", false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Toggle Console", NULL, false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Manual", NULL, false, true);
				ImGui::MenuItem("Release notes", NULL, false, true);
				ImGui::MenuItem("Report a bug", NULL, false, true);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		ImGui::Begin("test");
		ImGui::End();
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
	return new Sandbox();
}