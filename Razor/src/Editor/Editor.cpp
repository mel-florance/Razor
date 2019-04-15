#include "rzpch.h"
#include "imgui.h"
#include "Editor.h"

#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Geometry/Mesh.h"
#include "Razor/Application/Application.h"
#include "Razor/Materials/Texture.h"
#include "Razor/ECS/Manager.h"

#include "Razor/Ecs/Components/StaticMeshComponent.h"

namespace Razor {

	Editor::Editor(Engine* engine)
	{
		m_Engine = engine;
		m_ImGuiLayer = std::make_shared<ImGuiLayer>();
		tasksManager = new TasksManager();

		components["ProjectsManager"] = new ProjectsManager(this);
		components["AssetsManager"] = new AssetsManager(this);
		components["Console"] = new Console(this);
		components["MainMenu"] = new MainMenu(this);
		components["Outliner"] = new Outliner(this);
		components["PropertiesEditor"] = new PropertiesEditor(this);
		components["Tools"] = new Tools(this);
		components["Viewport"] = new Viewport(this);
		components["Logger"] = new Logger(this); 

		ProjectsManager* projManager = (ProjectsManager*)components["ProjectsManager"];
		projManager->loadProjects();

		ECS::Manager* ecsManager = new ECS::Manager();;
		auto& entity = ecsManager->createEntity();

		entity.addComponent<StaticMeshComponent>();
		entity.destroy();
		ecsManager->refresh();
	}

	void Editor::OnUpdate(float dt)
	{
		delta = dt;

		//if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		//	RZ_INFO("Tab key is pressed!(poll)");
	}

	void Editor::setupMeshBuffers(Node* node) {
		for (auto& mesh : node->meshes)
			mesh->setupBuffers();

		for (auto& n : node->nodes)
			setupMeshBuffers(n);
	}

	void Editor::importFinished(void* result)
	{
		Node* node = new Node(static_cast<Node*>(result));

		if (node != nullptr) {
			setupMeshBuffers(node);

			std::shared_ptr<Scene> scene = Application::Get().getScenesManager()->getActiveScene();
			scene->getSceneGraph()->addNode(node);

			RZ_INFO("Successfully imported model: {0}", node->name);
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_SPACE)
			{
				//Texture* texture = new Texture("./icon.png");

				Mesh mesh;

				AssetsManager::import(&mesh, &Editor::importFinished, Variant("data/Chair.fbx"));

				//tasksManager->add({ mesh,  &AssetsManager::import, &Editor::importFinished, Variant("data/house.fbx"), "Import task 1", 50 });
		/*		tasksManager->add({ &mesh1, &AssetsManager::import, &AssetsManager::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
				tasksManager->add({ &mesh2, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
				tasksManager->add({ &mesh3, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });*/
			}

			RZ_INFO("{0}", (char)e.GetKeyCode());
		}
	}

	void Editor::OnImGuiRender()
	{
		MainMenu* menu = (MainMenu*)components["MainMenu"];

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
			it->second->render(delta);

		if (menu != nullptr)
		{
			if (menu->show_create_project == true)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
				if (ImGui::Begin("Create Project", &menu->show_create_project, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					char name[32] = "";
					ImGui::Text("Project name:");
					ImGui::SameLine();
					ImGui::InputText("", name, 32, ImGuiInputTextFlags_CharsHexadecimal);
					ImGui::End();
				}
			}
		}

		ImGui::End();
	}

	Editor::~Editor()
	{

	}

}
