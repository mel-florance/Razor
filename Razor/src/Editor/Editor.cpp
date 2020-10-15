#include "rzpch.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Editor.h"

#include "Editor/Components/EditorViewport.h"
#include "Editor/Components/MainMenu.h"
#include "Editor/Components/Outliner.h"
#include "Editor/Components/Tools.h"
#include "Editor/Tools/Selection.h"
#include "Editor/Tools/Gizmo.h"

#include "Razor/Rendering/BillboardManager.h"
#include "Razor/Core/Engine.h"
#include "Razor/Core/System.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Scene/SceneGraph.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Application/Application.h"
#include "Razor/Materials/TextureAtlas.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/ECS/Manager.h"
#include "Razor/Lighting/LightBound.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"

#include "Razor/Ecs/Components/StaticMeshComponent.h"
#include "Razor/Physics/PhysicsBody.h"
#include "Razor/Physics/World.h"
#include "Razor/Types/Color.h"

#include "Razor/Audio/SoundsManager.h"
#include "Razor/Audio/Loaders/WAVLoader.h"

namespace fs = std::filesystem;

namespace Razor 
{

	IconsManager* Editor::icons_manager = nullptr;
	ComponentsManager* Editor::components_manager = nullptr;

	Editor::Editor(Engine* engine) :
		cam_type(0),
		delta(0.0f),
		system_audio_device_opened(true),
		system_graphic_device_opened(true),
		system_capabilities_opened(true),
		system_features_opened(true),
		editor_memory_opened(true),
		show_import_modal(false),
		current_category(PreferencesCategory::SYSTEM_INFORMATION)
	{
		m_Engine = engine;
		m_ImGuiLayer = std::make_shared<ImGuiLayer>(this);

		tps_camera = new TPSCamera(&Application::Get().GetWindow());
		fps_camera = new FPSCamera(&Application::Get().GetWindow());

		auto scene = Application::Get().getScenesManager()->getActiveScene();
		scene->addCamera(tps_camera);
		scene->addCamera(fps_camera);
		scene->setActiveCamera(tps_camera);

		tools_manager      = new ToolsManager(this);
		components_manager = new ComponentsManager(this);
		modals_manager     = new ModalsManager(this);
		icons_manager      = new IconsManager();

		/*ProjectsManager* projManager = (ProjectsManager*)components["ProjectsManager"];
		projManager->loadRecentProjects();*/

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


		//ModalsMap::iterator it_m;
		//for (it_m = modals.begin(); it_m != modals.end(); ++it_m)
		//	it_m->second->init();
	}

	void Editor::OnUpdate(float dt)
	{
		delta = dt;

		//if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		//	RZ_INFO("Tab key is pressed!(poll)");
	}

	std::shared_ptr<Node> Editor::import(const std::string& path)
	{
		Node* node = nullptr;
		//AssetsManager::import(node, &Editor::importFinished, Variant(path));

		return std::make_shared<Node>(node);
	}

	void Editor::importFinished(Node* result)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(result);
	
		if (node != nullptr)
		{
			node->setupMeshBuffers(node);

			std::shared_ptr<Scene> scene = Application::Get().getScenesManager()->getActiveScene();

			Log::trace("num meshes: %d", node->meshes.size());

			node->transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
			node->transform.setRotation(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));

			if (node->meshes.size() > 0)
			{
				node->meshes[0]->updateBoundings(node->transform);
				node->meshes[0]->getBoundingMesh()->setMaterial(ForwardRenderer::getColorMaterial());
				ForwardRenderer::addBoundingBox(node);
			}

			scene->getSceneGraph()->addNode(node);
			Log::info("Successfully imported model: %s", node->name.c_str());
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		components_manager->onEvent(event);

		/*ComponentsMap::iterator it_c;
		for (it_c = components.begin(); it_c != components.end(); ++it_c) {
			if (it_c->second->isActive())
				it_c->second->onEvent(event);
		}*/

		EditorViewport* vp = (EditorViewport*)getComponentsManager()->getComponent<EditorViewport>("Viewport");
		Outliner* outliner = (Outliner*)getComponentsManager()->getComponent<Outliner>("Outliner");
		Tools* tools = (Tools*)getComponentsManager()->getComponent<Tools>("Tools");

		Gizmo* gizmo = (Gizmo*)getToolsManager()->getTool<Gizmo>("gizmo");
		Selection* selection = getToolsManager()->getTool<Selection>("selection");

		if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonEvent& e = (MouseButtonEvent&)event;

			if ((e.GetMouseButton() == 0 && vp->isHovered()) && !(gizmo->isHovered()))
			{
				std::shared_ptr<Scene> scene = m_Engine->getScenesManager()->getActiveScene();

				ImVec2 mouse_pos = ImGui::GetMousePos();

				float offset = tools->isPanelVisible() ? tools->getSize().x : 0.0f;
				glm::vec2 vp_size = glm::vec2(vp->getSize().x - 3.0f, vp->getSize().y);

				World::RaycastResult result = World::RaycastResult();
				m_Engine->getPhysicsWorld()->raycast(&result, scene->getActiveCamera(), glm::vec2(mouse_pos.x - offset - 3.0f, mouse_pos.y - 53.0f), vp_size, 1000);

				if (result.hit)
				{
					std::shared_ptr<Node> node = scene->getSceneGraph()->getNodeById(result.node->id);

					if (node != nullptr)
					{
						if (Input::IsKeyPressed(RZ_KEY_LEFT_SHIFT))
						{
							if (selection->isSelected(node))
								selection->removeNode(node->id);
							else
								selection->addNode(node);
						}
						else
						{
							if (selection->isSelected(node))
								selection->removeNode(node->id);
							else
							{
								selection->clear();
								selection->addNode(node);
							}
						}
					}
					else
						Log::trace("%s not found", node->name.c_str());
				}
			}
		}

		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;
			EditorViewport* viewport = components_manager->getComponent<EditorViewport>("Viewport");

			if (e.GetKeyCode() == RZ_KEY_SPACE && vp->isHovered())
			{
				std::shared_ptr<Node> node = std::make_shared<Node>();
		/*		AssetsManager::import(node, &Editor::importFinished, Variant("./data/Jeep.fbx"));
*/
			
		/*		tasksManager->add({ &mesh1, &AssetsManager::import, &AssetsManager::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
				tasksManager->add({ &mesh2, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
				tasksManager->add({ &mesh3, &AssetsManager::import, &AssetsManager::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });*/
			}

			if (e.GetKeyCode() == RZ_KEY_Q && vp->isHovered() && Input::IsKeyPressed(RZ_KEY_LEFT_SHIFT))
				vp->showContextMenu();

			if (e.GetKeyCode() == RZ_KEY_C && vp->isHovered())
			{
				if (cam_type == 0) 
				{
					cam_type = 1;
					std::cout << "fps" << std::endl;
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
				vp->toggleProperties();
			}

			if (e.GetKeyCode() == RZ_KEY_T && viewport->isHovered())
			{
				tools->togglePanel();
			}

			if (e.GetKeyCode() == RZ_KEY_F11)
			{
				viewport->toggleFullscreen();

				ComponentsManager::ComponentsMap& components = components_manager->getComponents();
				ComponentsManager::ComponentsMap::iterator it_c = components_manager->getComponents().begin();

				for (; it_c != components.end(); ++it_c)
					if (it_c->first != "Viewport" && it_c->first != "MainMenu")
						it_c->second->setActive(!it_c->second->isActive());
			}

			if (e.GetKeyCode() == RZ_KEY_DELETE)
			{
				if (selection->getNodes().size() > 0) 
				{
					typedef std::vector<std::shared_ptr<Node>> node_list;

					node_list nodes = selection->getNodes();
					node_list::iterator it = nodes.begin();

					for (; it != nodes.end(); it++)
					{
						if (*it != nullptr)
						{
							for (auto light : (*it)->lights)
							{
								/*ForwardRenderer::billboard_manager->removeBillboard((*it)->id);
						
								ForwardRenderer* renderer = Application::Get().getForwardRenderer();
								renderer->removeLineMesh(light->getLightBound()->getNode());
								m_Engine->getScenesManager()->getActiveScene()->removeLight(light);*/
							}

							selection->removeNode((*it)->id);
							m_Engine->getPhysicsWorld()->removeNode((*it));

							for (auto m : (*it)->meshes) 
							{
								m->getBoundingMesh().reset();

								for (auto i : m->getInstances())
								{
									m_Engine->getPhysicsWorld()->getWorld()->removeRigidBody(i->body->getBody());
									m->removeInstance(i->index);
								}
							}

							std::vector<std::shared_ptr<Landscape>>::iterator it_land = (*it)->landscapes.begin();

							for (; it_land != (*it)->landscapes.end(); it_land++)
								(*it_land).reset();

							//ForwardRenderer::removeBoundingBox((*it)->id);
							m_Engine->getScenesManager()->getActiveScene()->getSceneGraph()->removeNode((*it)->id);
						}
					}
				}
			}

			if (e.GetKeyCode() == RZ_KEY_Q && viewport->isHovered()) {
				selection->clear();
			}

			Log::trace("%c", (char)e.GetKeyCode());
		}
	}

	void Editor::OnImGuiRender()
	{
		MainMenu* menu = components_manager->getComponent<MainMenu>("MainMenu");

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_DockNodeHost | ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("RazorEngine", &p_open, window_flags);
		ImGui::PopStyleVar();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MainDock");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		ComponentsManager::ComponentsMap components = components_manager->getComponents();
		ComponentsManager::ComponentsMap::iterator it_c = components.begin();
		for (; it_c != components.end(); it_c++) 
			if (it_c->second->isActive())
				it_c->second->render(delta);
	
		//ToolsMap::iterator it_t;
		//for (it_t = tools.begin(); it_t != tools.end(); ++it_t)
		//	if (it_t->second->isActive())
		//		it_t->second->render(delta);

		//ModalsMap::iterator it_m;
		//for (it_m = modals.begin(); it_m != modals.end(); ++it_m)
		//	if (it_m->second->isActive())
		//		it_m->second->render();

		if (menu != nullptr)
		{
			if (show_import_modal)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
				ImGui::SetNextWindowFocus();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

				if (ImGui::Begin("Import options", &show_import_modal, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse ))
				{
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::Columns(2, "twoColumns", true);

					static unsigned short initial_column_spacing = 0;
					if (initial_column_spacing < 2)
					{
						ImGui::SetColumnWidth(0, 120.0f);
						initial_column_spacing++;
					}

					ImGui::Text("ok");

					float width = ImGui::GetWindowWidth();
					float pos_y = ImGui::GetWindowHeight();

					ImGui::Columns(1);

					ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
					ImGui::PushItemWidth(-1);
					ImGui::Button("Import", ImVec2(width - 25.0f, 30.0f));
					ImGui::PopItemWidth();

					ImGui::End();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}

			if (menu->show_preferences)
			{
				ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
				ImGui::SetNextWindowFocus();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

				if (ImGui::Begin("Preferences", &menu->show_preferences, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
				{
					ImGui::Dummy(ImVec2(0, 5.0f));


					ImGui::Columns(2, "twoColumns", true);

					static unsigned short initial_column_spacing = 0;
					if (initial_column_spacing < 2)
					{
						ImGui::SetColumnWidth(0, 220.0f);
						initial_column_spacing++;
					}

					ImGui::PushItemWidth(-1);
					if (ImGui::ListBoxHeader("##List", 32, 30))
					{
						ImVec2 old = ImGui::GetFont()->DisplayOffset;
						ImGui::GetFont()->DisplayOffset.x = 25.0f;
						ImGui::GetFont()->DisplayOffset.y = 2.0f;

						ImGui::Selectable("System informations", current_category == PreferencesCategory::SYSTEM_INFORMATION, true, ImVec2(200.0f, 20.0f));

						if (ImGui::IsItemClicked())
							current_category = PreferencesCategory::SYSTEM_INFORMATION;

						ImVec2 old_pos_info = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(5, 3));
						icons_manager->drawIcon("info", glm::vec2(20.0f));
						ImGui::SetCursorPos(old_pos_info);

						ImGui::Selectable("Shortcuts", current_category == PreferencesCategory::SHORTCUTS, true, ImVec2(200.0f, 20.0f));

						if (ImGui::IsItemClicked())
							current_category = PreferencesCategory::SHORTCUTS;

						ImVec2 old_pos_joystick = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(5, 26));
						icons_manager->drawIcon("joystick", glm::vec2(20.0f));
						ImGui::SetCursorPos(old_pos_joystick);

						ImGui::Selectable("Engine", current_category == PreferencesCategory::ENGINE, false, ImVec2(200.0, 20.0f));

						if (ImGui::IsItemClicked())
							current_category = PreferencesCategory::ENGINE;

						ImVec2 old_pos_gear = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(5, 49));
						icons_manager->drawIcon("gear", glm::vec2(20.0f));
						ImGui::SetCursorPos(old_pos_gear);

						ImGui::Selectable("Theme", current_category == PreferencesCategory::THEME, false, ImVec2(200.0f, 20.0f));

						if (ImGui::IsItemClicked())
							current_category = PreferencesCategory::THEME;

						ImVec2 old_pos_theme = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(5, 70));
						icons_manager->drawIcon("brush", glm::vec2(20.0f));
						ImGui::SetCursorPos(old_pos_theme);


						ImVec2 old_pos_editor = ImGui::GetCursorPos();
						ImGui::SetCursorPos(ImVec2(5, 49));
						icons_manager->drawIcon("editor", glm::vec2(20.0f));
						ImGui::SetCursorPos(old_pos_editor);

						ImGui::Selectable("Editor", current_category == PreferencesCategory::EDITOR, false, ImVec2(200.0f, 20.0f));

						if (ImGui::IsItemClicked())
							current_category = PreferencesCategory::EDITOR;

						ImGui::GetFont()->DisplayOffset = old;
						ImGui::ListBoxFooter();
					}
					
					ImGui::PopItemWidth();
					ImGui::NextColumn();
					float col_width = 250.0f;

					if (current_category == PreferencesCategory::SYSTEM_INFORMATION)
					{
						Engine::SystemInfos infos = m_Engine->getSystemInfos();

						ImGui::SetNextTreeNodeOpen(system_graphic_device_opened);
						if (Utils::treeNode("Graphic device"))
						{
							system_graphic_device_opened = true;
							ImGui::Dummy(ImVec2(0, 5.0f));

							ImGui::Text("Vendor");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text((char*)infos.vendor);
							ImGui::Indent(-col_width);

							ImGui::Text("Renderer");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text((char*)infos.renderer);
							ImGui::Indent(-col_width);

							ImGui::Text("OpenGL version");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text((char*)infos.opengl_version);
							ImGui::Indent(-col_width);

							ImGui::Text("Shading language version");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text((char*)infos.shading_language_version);
							ImGui::Indent(-col_width);

							ImGui::Dummy(ImVec2(0, 10.0f));
							ImGui::TreePop();
						}
						else
							system_graphic_device_opened = false;

						ImGui::SetNextTreeNodeOpen(system_audio_device_opened);
						if (Utils::treeNode("Audio device"))
						{
							SoundsManager::DeviceInfo audio_infos = m_Engine->getSoundsManager()->getDeviceInfos();

							system_audio_device_opened = true;
							ImGui::Dummy(ImVec2(0, 5.0f));

							ImGui::Text("Vendor");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(audio_infos.vendor);
							ImGui::Indent(-col_width);

							ImGui::Text("Renderer");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(audio_infos.renderer);
							ImGui::Indent(-col_width);

							ImGui::Text("Version");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(audio_infos.version);
							ImGui::Indent(-col_width);

							ImGui::Dummy(ImVec2(0, 10.0f));
							ImGui::TreePop();
						}
						else
							system_audio_device_opened = false;

						ImGui::SetNextTreeNodeOpen(system_capabilities_opened);
						if (Utils::treeNode("Capabilities"))
						{
							system_capabilities_opened = true;
							ImGui::Dummy(ImVec2(0, 5.0f));

							ImGui::Text("Max texture image units");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_texture_image_units).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max combined texture image units");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_combined_texture_image_units).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max vertex texture image units");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_vertex_texture_image_units).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max texture size");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_texture_size).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max cubemap texture size");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_cubemap_texture_size).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max render buffer size");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_render_buffer_size).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max vertex attributes");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_vertex_attributes).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max varying vectors");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_varying_vectors).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max fragment uniform vectors");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_fragment_uniform_vectors).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Max vertex uniform vectors");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.max_vertex_uniform_vectors).c_str());
							ImGui::Indent(-col_width);

							ImGui::Dummy(ImVec2(0, 10.0f));
							ImGui::TreePop();
						}
						else
							system_capabilities_opened = false;

						ImGui::SetNextTreeNodeOpen(system_features_opened);
						if (Utils::treeNode("Features"))
						{
							system_features_opened = true;
							ImGui::Dummy(ImVec2(0, 5.0f));

							ImGui::Text("Active texture unit");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.active_texture_unit - 33984).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Aliased line width range");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.aliased_line_width_range).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Smooth line width range");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.smooth_line_width_range).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Array buffer binding");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.array_buffer_binding).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Blend");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(std::to_string(infos.blend).c_str());
							ImGui::Indent(-col_width);

							ImGui::Dummy(ImVec2(0, 10.0f));
							ImGui::TreePop();
						}
						else
							system_features_opened = false;
					}
					else if (current_category == PreferencesCategory::ENGINE)
					{

					}
					else if (current_category == PreferencesCategory::EDITOR)
					{
						ImGui::SetNextTreeNodeOpen(editor_memory_opened);
						if (Utils::treeNode("Memory"))
						{
							editor_memory_opened = true;
							System* system = m_Engine->getSystem();
							system->getStats();

							System::VirtualMemory& vm = system->getVirtualMemory();
							System::PhysicalMemory& pm = system->getPhysicalMemory();

							ImGui::Dummy(ImVec2(0, 5.0f));

							ImGui::Text("Total virtual memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(vm.total).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("System usage virtual memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(vm.system_usage).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Process usage virtual memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(vm.process_usage).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Total physical memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(pm.total).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("System usage physical memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(pm.system_usage).c_str());
							ImGui::Indent(-col_width);

							ImGui::Text("Process physical memory");
							ImGui::SameLine();
							ImGui::Indent(col_width);
							ImGui::Text(Utils::bytesToSize(pm.process_usage).c_str());
							ImGui::Indent(-col_width);

							ImGui::TreePop();
						}
						else
							editor_memory_opened = false;
					}
					else if (current_category == PreferencesCategory::SHORTCUTS)
					{

					}
					else if (current_category == PreferencesCategory::THEME)
					{
						ImGuiIO& io = ImGui::GetIO();
						ImFont* font_current = ImGui::GetFont();
		

						if (ImGui::BeginCombo("Font", font_current->GetDebugName()))
						{
							for (int n = 0; n < io.Fonts->Fonts.Size; n++)
							{
								ImFont* font = io.Fonts->Fonts[n];
								Log::trace("%s", font->GetDebugName());
								ImGui::PushID((void*)font);

								if (ImGui::Selectable(font->GetDebugName(), font == font_current))
									io.FontDefault = font;

								ImGui::PopID();
							}

							ImGui::EndCombo();
						}
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
