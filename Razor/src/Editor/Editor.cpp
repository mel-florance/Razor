#include "rzpch.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Editor.h"

#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Application/Application.h"
#include "Razor/Materials/TextureAtlas.h"
#include "Razor/ECS/Manager.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"

#include "Razor/Ecs/Components/StaticMeshComponent.h"
#include "Razor/Physics/PhysicsBody.h"
#include "Razor/Physics/World.h"
#include "Razor/Types/Color.h"

#include "Razor/Audio/Loaders/WAVLoader.h"

namespace Razor 
{
	TextureAtlas* Editor::icons_atlas = nullptr;

	Editor::Editor(Engine* engine) :
		cam_type(0),
		delta(0.0f)
	{
		m_Engine = engine;
		m_ImGuiLayer = std::make_shared<ImGuiLayer>(this);
		tasksManager = new TasksManager();

		tps_camera = new TPSCamera(&Application::Get().GetWindow());
		fps_camera = new FPSCamera(&Application::Get().GetWindow());

		auto scene = Application::Get().getScenesManager()->getActiveScene();
		scene->addCamera(tps_camera);
		scene->addCamera(fps_camera);
		scene->setActiveCamera(tps_camera);

		icons_atlas = new TextureAtlas("./data/icons.png", false, false);

		icons_atlas->addItem("sequencer",         0, 0);
		icons_atlas->addItem("texture",           0, 1);
		icons_atlas->addItem("image_curve",       0, 2);
		icons_atlas->addItem("image_colors",      0, 3);
		icons_atlas->addItem("image_histo",       0, 4);
		icons_atlas->addItem("texture_mini",      0, 5);
		icons_atlas->addItem("plane",             1, 0);
		icons_atlas->addItem("sphere",            1, 1);
		icons_atlas->addItem("cube",              1, 2);
		icons_atlas->addItem("calendar",          2, 4);
		icons_atlas->addItem("node",              5, 19);
		icons_atlas->addItem("node_hover",        5, 20);
		icons_atlas->addItem("point_light",       18, 12);
		icons_atlas->addItem("directional_light", 18, 13);
		icons_atlas->addItem("spot_light",        18, 14);
		icons_atlas->addItem("camera",            19, 6);
		icons_atlas->addItem("noisy_curve",       8, 18);

		tools["selection"] = new Selection(this);
		tools["gizmo"] = new Gizmo(this);
		tools["grid_axis"] = new GridAxis(this);

		components["ProjectsManager"]  = new ProjectsManager(this);
		components["AssetsManager"]    = new AssetsManager(this);
		components["Logger"]           = new Logger(this);
		components["Console"]          = new Console(this);
		components["MainMenu"]         = new MainMenu(this);
		components["Outliner"]         = new Outliner(this);
		components["PropertiesEditor"] = new PropertiesEditor(this);
		components["Tools"]            = new Tools(this);
		components["Viewport"]         = new Viewport(this);

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
	}

	void Editor::OnUpdate(float dt)
	{
		delta = dt;

		//if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		//	RZ_INFO("Tab key is pressed!(poll)");
	}

	void Editor::setupMeshBuffers(std::shared_ptr<Node> node) {
		for (auto mesh : node->meshes)
			mesh->setupBuffers();

		for (auto n : node->nodes)
			setupMeshBuffers(n);
	}

	void Editor::drawIcon(const std::string& name, const glm::vec2& size)
	{
		auto extents = icons_atlas->getItemPosition(name);

		ImGui::Image(
			(void*)(intptr_t)icons_atlas->getId(),
			ImVec2(size.x, size.y),
			ImVec2(extents.x, extents.y),
			ImVec2(extents.z, extents.w));
	}

	void Editor::drawButtonIcon(const std::string& name, const glm::vec2& size)
	{
		auto extents = icons_atlas->getItemPosition(name);

		ImGui::ImageButton(
			(void*)(intptr_t)icons_atlas->getId(),
			ImVec2(size.x, size.y),
			ImVec2(extents.x, extents.y),
			ImVec2(extents.z, extents.w)
		);
	}

	std::shared_ptr<Node> Editor::import(const std::string& path)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>();
		AssetsManager::import(node, &Editor::importFinished, Variant(path));

		return node;
	}

	void Editor::importFinished(std::shared_ptr<Node> result)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(std::move(result));
	
		if (node != nullptr) 
		{
			setupMeshBuffers(node);
			std::shared_ptr<Scene> scene = Application::Get().getScenesManager()->getActiveScene();

			Log::trace("num meshes: %d", node->meshes.size());

			//node->transform.setScale(glm::vec3(0.004f, 0.004f, 0.004f));
			//node->transform.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
			node->transform.setRotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));

			scene->getSceneGraph()->addNode(node);
			Log::info("Successfully imported model: %s", node->name.c_str());
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;
			Viewport* viewport = (Viewport*)components["Viewport"];

			if (e.GetKeyCode() == RZ_KEY_SPACE && viewport->isHovered())
			{
				/*std::shared_ptr<Node> node = std::make_shared<Node>();
				AssetsManager::import(node, &Editor::importFinished, Variant("./data/Jeep.blend"));*/

				//tasksManager->add({ node,  &AssetsManager::import, &Editor::importFinished, Variant("./data/sponza.obj"), "Import task 1", 50 });
		/*		tasksManager->add({ &mesh1, &AssetsManager::import, &AssetsManager::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
				tasksManager->add({ &mesh2, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
				tasksManager->add({ &mesh3, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });*/
			}

			if (e.GetKeyCode() == RZ_KEY_Q && viewport->isHovered() && Input::IsKeyPressed(RZ_KEY_LEFT_SHIFT))
				viewport->showContextMenu();

			if (e.GetKeyCode() == RZ_KEY_C && viewport->isHovered())
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
			
			TPSCamera* cam = (TPSCamera*)Application::Get().getScenesManager()->getActiveScene()->getActiveCamera();
		

			if (e.GetKeyCode() == RZ_KEY_KP_7 && cam_type == 0 && Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL) && viewport->isHovered())
			{
				cam->setPitch(-90.0f);
				cam->setAngle(90.0f);
				cam->setRoll(0.0f);
			}
			else if (e.GetKeyCode() == RZ_KEY_KP_7 && cam_type == 0 && viewport->isHovered())
			{
				cam->setPitch(90.0f);
				cam->setAngle(90.0f);
				cam->setRoll(0.0f);
			}


			if (e.GetKeyCode() == RZ_KEY_KP_3 && cam_type == 0 && Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL) && viewport->isHovered())
			{
				cam->setPitch(0.0f);
				cam->setAngle(-90.0f);
				cam->setRoll(0.0f);
			}
			else if (e.GetKeyCode() == RZ_KEY_KP_3 && cam_type == 0 && viewport->isHovered())
			{
				cam->setPitch(0.0f);
				cam->setAngle(90.0f);
				cam->setRoll(0.0f);
			}


			if (e.GetKeyCode() == RZ_KEY_KP_1 && cam_type == 0 && Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL) &&viewport->isHovered())
			{
				cam->setPitch(0.0f);
				cam->setAngle(180.0f);
				cam->setRoll(0.0f);
			}
			else if (e.GetKeyCode() == RZ_KEY_KP_1 && cam_type == 0 && viewport->isHovered())
			{
				cam->setPitch(0.0f);
				cam->setAngle(0.0f);
				cam->setRoll(0.0f);
			}


			if (e.GetKeyCode() == RZ_KEY_N && viewport->isHovered())
			{
				Viewport* vp = (Viewport*)components["Viewport"];
				vp->toggleProperties();
			}

			if (e.GetKeyCode() == RZ_KEY_T && viewport->isHovered())
			{
				Tools* tools_panel = (Tools*)components["Tools"];
				tools_panel->togglePanel();
			}

			if (e.GetKeyCode() == RZ_KEY_F11)
			{
				ComponentsMap::iterator it_c;
				for (it_c = components.begin(); it_c != components.end(); ++it_c)
					if (it_c->first != "Viewport" && it_c->first != "MainMenu")
						it_c->second->setActive(!it_c->second->isActive());
			}

			if (e.GetKeyCode() == RZ_KEY_DELETE || e.GetKeyCode() == RZ_KEY_X)
			{
				Selection* selection = getTool<Selection*>("selection");
				Outliner* outliner = getComponent<Outliner*>("Outliner");

				if (selection->getNodes().size() > 0) 
				{
					for (auto node : selection->getNodes())
					{
						for (auto light : node->lights)
							m_Engine->getScenesManager()->getActiveScene()->removeLight(light);

						for (auto mesh : node->meshes)
							mesh.reset();

						selection->removeNode(node->id);
						m_Engine->getPhysicsWorld()->removeNode(node);

						for (auto m : node->meshes) {
							for (auto i : m->getInstances()) {
								m_Engine->getPhysicsWorld()->getWorld()->removeRigidBody(i->body->getBody());
							}
						}

						m_Engine->getScenesManager()->getActiveScene()->getSceneGraph()->removeNode(node->id);
					}
				}
			}

			if (e.GetKeyCode() == RZ_KEY_Q && viewport->isHovered()) {
				Selection* selection = getTool<Selection*>("selection");
				selection->clear();
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
		for (it_c = components.begin(); it_c != components.end(); ++it_c) {
			if(it_c->second->isActive())
				it_c->second->render(delta);
		}

		ToolsMap::iterator it_t;
		for (it_t = tools.begin(); it_t != tools.end(); ++it_t) {
			if (it_t->second->isActive())
				it_t->second->render(delta);
		}

		if (menu != nullptr)
		{
			if (menu->show_create_project == true)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
				ImGui::SetNextWindowFocus();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

				if (ImGui::Begin("Create Project", &menu->show_create_project, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::Columns(2, "twoColumns", true);

					static unsigned short initial_column_spacing = 0;
					if (initial_column_spacing < 2)
					{
						ImGui::SetColumnWidth(0, 120.0f);
						initial_column_spacing++;
					}

					char name[32] = "";
					ImGui::Indent(10.0f);
					ImGui::Text("Project name ");
					ImGui::Indent(-10.0f);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Indent(10.0f);
					ImGui::InputText("", name, 32);
					ImGui::Indent(-10.0f);
					ImGui::PopItemWidth();
				
					float width = ImGui::GetWindowWidth();
					float pos_y = ImGui::GetWindowHeight();

					ImGui::Columns(1);

					ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
					ImGui::PushItemWidth(-1);
					ImGui::Button("Create project", ImVec2(width - 25.0f, 30.0f));
					ImGui::PopItemWidth();

					ImGui::End();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}
			
			if (menu->show_preferences)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
				ImGui::SetNextWindowFocus();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

				if (ImGui::Begin("Preferences", &menu->show_preferences, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::Columns(2, "twoColumns", true);

					static unsigned short initial_column_spacing = 0;
					if (initial_column_spacing < 2)
					{
						ImGui::SetColumnWidth(0, 120.0f);
						initial_column_spacing++;
					}

					ImGui::End();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}
		}

		ImGui::End();
	}

	Editor::~Editor()
	{

	}

}
