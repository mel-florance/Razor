#include "rzpch.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Tools.h"
#include "Razor/Geometry/Geometry.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/Materials/Presets/PbrMaterial.h"
#include "Editor/Tools/Selection.h"

#include "Razor/Physics/World.h"
#include "Razor/Physics/Bodies/CubePhysicsBody.h"
#include "Razor/Physics/Bodies/PlanePhysicsBody.h"
#include "Razor/Physics/Bodies/SpherePhysicsBody.h"

#include "Editor/Components/AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Lighting/LightBound.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Lighting/Spot.h"
#include "Razor/Rendering/ForwardRenderer.h"

namespace Razor 
{

	Tools::Tools(Editor* editor) :
		EditorComponent(editor),
		primitives_opened(true),
		lights_opened(true),
		cameras_opened(true),
		physics_opened(true),
		landscape_opened(true),
		show_cube_props(false),
		show_uvsphere_props(false),
		show_plane_props(false),
		show_directional_props(false),
		show_point_props(false),
		show_spot_props(false),
		show_landscape_props(false),
		panel_visible(true),
		cube_parameters(CubeParameters()),
		UVsphere_parameters(UVSphereParameters()),
		plane_parameters(PlaneParameters()),
		directional_parameters(DirectionalParameters()),
		point_parameters(PointParameters()),
		spot_parameters(SpotParameters())
	{
		selection = editor->getToolsManager()->getTool<Selection>("selection");
		checkerMap = new Texture("./data/checker.png", true);
	}

	Tools::~Tools()
	{
	}

	void Tools::render(float delta)
	{
		if (panel_visible)
		{
			ImGui::Begin("Tools");

			auto win_size = ImGui::GetWindowSize();
			size = glm::vec2(win_size.x, win_size.y);

			ImGui::SetNextTreeNodeOpen(primitives_opened);
			auto btn_size = ImVec2(size.x - 57.0f, 25.0f);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


			if (Utils::treeNode("Primitives"))
			{
				primitives_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("cube", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Cube", btn_size);
				ImGui::Indent(-17.0f);

				if (ImGui::IsItemClicked())
					show_cube_props = true;

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
			
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("sphere", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("UV Sphere", btn_size); 
				ImGui::Indent(-17.0f);

				if (ImGui::IsItemClicked())
					show_uvsphere_props = true;

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("plane", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Plane", btn_size);
				ImGui::Indent(-17.0f);;

				if (ImGui::IsItemClicked())
					show_plane_props = true;

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				primitives_opened = false;



			ImGui::PopStyleVar();

			if (Utils::treeNode("Lights"))
			{
				lights_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("directional_light", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Directional", btn_size);

				if (ImGui::IsItemClicked())
					show_directional_props = true;

				ImGui::Indent(-17.0f);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("point_light", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Point", btn_size);

				if (ImGui::IsItemClicked())
					show_point_props = true;

				ImGui::Indent(-17.0f);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("spot_light", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Spot", btn_size);

				if (ImGui::IsItemClicked())
					show_spot_props = true;

				ImGui::Indent(-17.0f);

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				lights_opened = false;

			if (Utils::treeNode("Cameras"))
			{
				cameras_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("camera", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Free camera", btn_size);
				ImGui::Indent(-17.0f);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("camera", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Orbital camera", btn_size);
				ImGui::Indent(-17.0f);

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				cameras_opened = false;

			if (Utils::treeNode("Physics"))
			{
				physics_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				physics_opened = false;

			if (Utils::treeNode("Landscape"))
			{
				landscape_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				editor->icons_manager->drawIcon("noisy_curve", glm::vec2(25.0f), IconsManager::IconType::BUTTON);
				ImGui::PopItemFlag();
				ImGui::SameLine();
				ImGui::Indent(17.0f);
				ImGui::Button("Landscape", btn_size);
				ImGui::Indent(-17.0f);

				if (ImGui::IsItemClicked())
					show_landscape_props = true;

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				landscape_opened = false;
			
			//editor->icons_manager->drawIcon("cube", glm::vec2(60.0f), IconsManager::IconType::BUTTON);

			ImGui::End();
		}

		if (show_landscape_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add landscape", &show_landscape_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				// PROPS HERE

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create landscape", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Cube_x";
					std::shared_ptr<Cube> cube = std::make_shared<Cube>(cube_parameters.radius);
					std::shared_ptr<PhongMaterial> mat = std::make_shared<PhongMaterial>();
					mat->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
					cube->setPhysicsBody(new CubePhysicsBody(node.get(), glm::vec3(1.0f)));

					cube->setMaterial(mat);

					node->transform.setRotation(glm::vec3(
						glm::radians(cube_parameters.rotation.x),
						glm::radians(cube_parameters.rotation.y),
						glm::radians(cube_parameters.rotation.z)
					));

					node->transform.setPosition(cube_parameters.position);
					node->meshes.push_back(cube);
					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getPhysicsWorld()->addNode(node);
					show_landscape_props = false;
				}

				ImGui::End();
			}
		}

		if (show_cube_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add cube", &show_cube_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Radius");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat("##Radius", &cube_parameters.radius);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Position");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Position", &cube_parameters.position[0]);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Rotation");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Rotation", &cube_parameters.rotation[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create cube", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Cube_x";
					std::shared_ptr<Cube> cube = std::make_shared<Cube>(cube_parameters.radius);
					std::shared_ptr<PbrMaterial> mat = std::make_shared<PbrMaterial>();
					mat->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
			

					cube->setMaterial(mat);

					node->transform.setRotation(glm::vec3(
						glm::radians(cube_parameters.rotation.x),
						glm::radians(cube_parameters.rotation.y),
						glm::radians(cube_parameters.rotation.z)
					));

					node->transform.setPosition(cube_parameters.position);
					cube->setPhysicsBody(new CubePhysicsBody(node.get(), glm::vec3(cube_parameters.radius)));
					node->meshes.push_back(cube);

					cube->updateBoundings(node->transform);
					cube->getBoundingMesh()->setMaterial(ForwardRenderer::getColorMaterial());
					ForwardRenderer::addBoundingBox(node);

					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getPhysicsWorld()->addNode(node);
					show_cube_props = false;
				}

				ImGui::End();
			}
		}

		if (show_uvsphere_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add UVSphere", &show_uvsphere_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Radius");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat("##Radius", &UVsphere_parameters.radius);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Position");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Position", &UVsphere_parameters.position[0]);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Rotation");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Rotation", &UVsphere_parameters.rotation[0]);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Segments");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragInt2("##Segments", &UVsphere_parameters.segments[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create UVSphere", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "UVSphere";
					std::shared_ptr<UVSphere> uvsphere = std::make_shared<UVSphere>(UVsphere_parameters.radius, UVsphere_parameters.segments);

					std::shared_ptr<PbrMaterial> mat = std::make_shared<PbrMaterial>();
					mat->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
					uvsphere->setMaterial(mat);

					node->transform.setPosition(UVsphere_parameters.position);
		
					node->transform.setRotation(glm::vec3(
						glm::radians(UVsphere_parameters.rotation.x),
						glm::radians(UVsphere_parameters.rotation.y),
						glm::radians(UVsphere_parameters.rotation.z)
					));

					uvsphere->setPhysicsBody(new SpherePhysicsBody(node.get(), UVsphere_parameters.radius));
					node->meshes.push_back(uvsphere);

					uvsphere->updateBoundings(node->transform);
					uvsphere->getBoundingMesh()->setMaterial(ForwardRenderer::getColorMaterial());
					ForwardRenderer::addBoundingBox(node);

					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getPhysicsWorld()->addNode(node);
					show_uvsphere_props = false;
				}

				ImGui::End();
			}
		}

		if (show_plane_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add Plane", &show_plane_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Radius");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat("##Radius", &plane_parameters.radius);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Position");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Position", &plane_parameters.position[0]);
				ImGui::PopItemWidth();

				ImGui::NextColumn();
				ImGui::Text("Rotation");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Rotation", &plane_parameters.rotation[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create Plane", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Plane";
					std::shared_ptr<Plane> plane = std::make_shared<Plane>(plane_parameters.radius);

					std::shared_ptr<PbrMaterial> mat = std::make_shared<PbrMaterial>();
					mat->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
					plane->setMaterial(mat);
				
					node->transform.setRotation(glm::vec3(
						glm::radians(plane_parameters.rotation.x),
						glm::radians(plane_parameters.rotation.y),
						glm::radians(plane_parameters.rotation.z)
					));

					node->transform.setPosition(plane_parameters.position);
					plane->setPhysicsBody(new PlanePhysicsBody(node.get(), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));
					node->meshes.push_back(plane);
					

					plane->updateBoundings(node->transform);
					plane->getBoundingMesh()->setMaterial(ForwardRenderer::getColorMaterial());
					ForwardRenderer::addBoundingBox(node);

					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getPhysicsWorld()->addNode(node);
					show_plane_props = false;
				
				}

				ImGui::End();
			}
		}

		if (show_directional_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add directional light", &show_directional_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Direction");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Direction", &directional_parameters.direction[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create directional light", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Directional light";
					std::shared_ptr<Directional> directional = std::make_shared<Directional>(editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera());

					directional->setDirection(glm::vec3(
						glm::radians(plane_parameters.rotation.x),
						glm::radians(plane_parameters.rotation.y),
						glm::radians(plane_parameters.rotation.z)
					));

					node->lights.push_back(directional);
					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->addLight(directional, Light::Type::DIRECTIONAL);
					show_directional_props = false;
				}

				ImGui::End();
			}
		}

		if (show_point_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add directional light", &show_point_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Position");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Position", &point_parameters.position[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create point light", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Point light";
					std::shared_ptr<Point> point = std::make_shared<Point>(editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera());

					point->setPosition(glm::vec3(
						glm::radians(point_parameters.position.x),
						glm::radians(point_parameters.position.y),
						glm::radians(point_parameters.position.z)
					));

					node->lights.push_back(point);
					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->addLight(point, Light::Type::POINT);
					show_point_props = false;
				}

				ImGui::End();
			}
		}

		if (show_spot_props)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Add spot light", &show_spot_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Indent(10.0f);
				ImGui::Text("Position");
				ImGui::NextColumn();

				float margin_right = 18.0f;

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Position", &spot_parameters.position[0]);
				ImGui::PopItemWidth();

				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetColumnWidth() - margin_right);
				ImGui::DragFloat3("##Direction", &spot_parameters.direction[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("Create spot light", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::shared_ptr<Node> node = std::make_shared<Node>();
					node->name = "Spot light";
					std::shared_ptr<Spot> spot = std::make_shared<Spot>(editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera());

					spot->setPosition(glm::vec3(
						glm::radians(spot_parameters.position.x),
						glm::radians(spot_parameters.position.y),
						glm::radians(spot_parameters.position.z)
					));

					spot->setDirection(glm::vec3(
						glm::radians(spot_parameters.direction.x),
						glm::radians(spot_parameters.direction.y),
						glm::radians(spot_parameters.direction.z)
					));

					node->lights.push_back(spot);
					selection->clear();
					selection->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
					editor->getEngine()->getScenesManager()->getActiveScene()->addLight(spot, Light::Type::SPOT);
					show_spot_props = false;
				}

				ImGui::End();
			}
		}
	}

	void Tools::togglePanel()
	{
		panel_visible = !panel_visible;
	}

}