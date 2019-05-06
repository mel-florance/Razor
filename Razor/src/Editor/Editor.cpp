#include "rzpch.h"
#include "imgui.h"
#include "Editor.h"

#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Application/Application.h"
#include "Razor/Materials/Texture.h"
#include "Razor/ECS/Manager.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"

#include "Razor/Ecs/Components/StaticMeshComponent.h"
#include "Razor/Types/Color.h"

namespace Razor 
{

	Editor::Editor(Engine* engine) : cam_type(0)
	{
		m_Engine = engine;
		m_ImGuiLayer = std::make_shared<ImGuiLayer>(this);
		tasksManager = new TasksManager();

		tools["selection"] = new Selection(this);
		tools["gizmo"] = new Gizmo(this);
		tools["grid_axis"] = new GridAxis(this);

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

		//ECS::Manager* ecsManager = new ECS::Manager();;
		//auto& entity = ecsManager->createEntity();

		//entity.addComponent<StaticMeshComponent>();
		//entity.destroy();
		//ecsManager->refresh();

		//Color::RGB color(131, 82, 31);
		//Color::HSL h = color.toHSL();

		//std::cout << h.h << std::endl;
		//std::cout << h.s << std::endl;
		//std::cout << h.l << std::endl;

		tps_camera = new TPSCamera(&Application::Get().GetWindow());
		fps_camera = new FPSCamera(&Application::Get().GetWindow());

		auto scene = Application::Get().getScenesManager()->getActiveScene();
		scene->addCamera(tps_camera);
		scene->addCamera(fps_camera);
		scene->setActiveCamera(tps_camera);
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

		if (node != nullptr) 
		{
			setupMeshBuffers(node);
			std::shared_ptr<Scene> scene = Application::Get().getScenesManager()->getActiveScene();

			node->transform.setScale(glm::vec3(0.005f, 0.005f, 0.005f));
			//node->transform.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
			//node->transform.setRotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));

			scene->getSceneGraph()->addNode(node);
			Log::info("Successfully imported model: %s", node->name.c_str());
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_SPACE)
			{
				StaticMesh mesh;
				AssetsManager::import(&mesh, &Editor::importFinished, Variant("data/sponza.obj"));

				//tasksManager->add({ mesh,  &AssetsManager::import, &Editor::importFinished, Variant("data/house.fbx"), "Import task 1", 50 });
		/*		tasksManager->add({ &mesh1, &AssetsManager::import, &AssetsManager::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
				tasksManager->add({ &mesh2, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
				tasksManager->add({ &mesh3, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });*/
			}

			if (e.GetKeyCode() == RZ_KEY_C)
			{
				if (cam_type == 0) 
				{
					cam_type = 1;
					Application::Get().getScenesManager()->getActiveScene()->setActiveCamera(fps_camera);
				}
				else 
				{
					cam_type = 0;
					Application::Get().getScenesManager()->getActiveScene()->setActiveCamera(tps_camera);
				}
			}

			if (e.GetKeyCode() == RZ_KEY_N)
			{
				Viewport* vp = (Viewport*)components["Viewport"];
				vp->toggleProperties();
			}

			if (e.GetKeyCode() == RZ_KEY_T)
			{
				Tools* tools_panel = (Tools*)components["Tools"];
				tools_panel->togglePanel();
			}


			//Log::trace("%c", (char)e.GetKeyCode());
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

		ComponentsMap::iterator it_c;
		for (it_c = components.begin(); it_c != components.end(); ++it_c)
			it_c->second->render(delta);

		ToolsMap::iterator it_t;
		for (it_t = tools.begin(); it_t != tools.end(); ++it_t)
			it_t->second->render(delta);

		if (menu != nullptr)
		{
			if (menu->show_create_project == true)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
				ImGui::SetNextWindowFocus();

				if (ImGui::Begin("Create Project", &menu->show_create_project, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					ImGui::Columns(2, "twoColumns", true);

					char name[32] = "";
					ImGui::Text("Project name ");

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
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
