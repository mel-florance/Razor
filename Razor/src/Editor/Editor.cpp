#include "rzpch.h"
#include "imgui.h"
#include "Editor.h"

#include "Razor/Geometry/Mesh.h"
#include "Razor/Application/Application.h"
#include "Razor/Materials/Texture.h"

namespace Razor {

	Editor::Editor()
	{
		m_ImGuiLayer = std::make_shared<ImGuiLayer>();
		tasksManager = new TasksManager();

		components["AssetsManager"] = new AssetsManager(this);
		components["Console"] = new Console(this);
		components["MainMenu"] = new MainMenu(this);
		components["Outliner"] = new Outliner(this);
		components["PropertiesEditor"] = new PropertiesEditor(this);
		components["Tools"] = new Tools(this);
		components["Viewport"] = new Viewport(this);
		components["Logger"] = new Logger(this); 
	}

	void Editor::OnUpdate()
	{
		if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		{
			RZ_INFO("Tab key is pressed!(poll)");
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_SPACE)
			{
				Texture* texture = new Texture("./icon.png");

				Mesh mesh, mesh1, mesh2, mesh3;
				tasksManager->add({ &mesh,  &AssetsManager::import, &AssetsManager::finished, Variant("data/house.fbx"), "Import task 1", 50 });
				tasksManager->add({ &mesh1, &AssetsManager::import, &AssetsManager::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
				tasksManager->add({ &mesh2, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
				tasksManager->add({ &mesh3, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });
			}

			RZ_INFO("{0}", (char)e.GetKeyCode());
		}
	}

	void Editor::OnImGuiRender()
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

		ComponentsMap::iterator it;
		for (it = components.begin(); it != components.end(); ++it)
			it->second->render();

		ImGui::End();
	}

	Editor::~Editor()
	{

	}

}
