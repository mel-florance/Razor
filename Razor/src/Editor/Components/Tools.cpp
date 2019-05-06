#include "rzpch.h"
#include "imgui.h"
#include "Tools.h"
#include "Razor/Geometry/Geometry.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Materials/Presets/PhongMaterial.h"

namespace Razor {

	Tools::Tools(Editor* editor) : 
		EditorComponent(editor),
		primitives_opened(true),
		lights_opened(true),
		cameras_opened(true),
		show_cube_props(false),
		show_uvsphere_props(false),
		show_plane_props(false),
		panel_visible(true),
		cube_parameters(CubeParameters()),
		UVsphere_parameters(UVSphereParameters()),
		plane_parameters(PlaneParameters())
	{
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
			ImGui::Dummy(ImVec2(0, 5.0f));
			auto btn_size = ImVec2(size.x - 30.0f, 25.0f);

			if (Utils::treeNode("Primitives"))
			{
				primitives_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Cube", btn_size);

				if (ImGui::IsItemClicked())
					show_cube_props = true;

				if (show_cube_props)
				{
					ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
					ImGui::SetNextWindowFocus();

					if (ImGui::Begin("Add cube", &show_cube_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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
							Node* node = new Node();
							node->name = "Cube_x";
							Cube* cube = new Cube(cube_parameters.radius);
							cube->setMaterial(new PhongMaterial());

							node->transform.setRotation(glm::vec3(
								glm::radians(cube_parameters.rotation.x),
								glm::radians(cube_parameters.rotation.y),
								glm::radians(cube_parameters.rotation.z)
							));
							
							node->transform.setPosition(cube_parameters.position);
							node->meshes.push_back(cube);
							editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
							show_cube_props = false;
						}

						ImGui::End();
					}
				}

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Sphere", btn_size);

				if (ImGui::IsItemClicked())
					show_uvsphere_props = true;

				if (show_uvsphere_props)
				{
					ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
					ImGui::SetNextWindowFocus();

					if (ImGui::Begin("Add UVSphere", &show_uvsphere_props, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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
							Node* node = new Node();
							node->name = "UVSphere";
							UVSphere* uvsphere = new UVSphere(UVsphere_parameters.radius, UVsphere_parameters.segments);
							uvsphere->setMaterial(new PhongMaterial());

							node->transform.setRotation(glm::vec3(
								glm::radians(UVsphere_parameters.rotation.x),
								glm::radians(UVsphere_parameters.rotation.y),
								glm::radians(UVsphere_parameters.rotation.z)
							));

							node->transform.setPosition(UVsphere_parameters.position);
							node->meshes.push_back(uvsphere);
							editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
							show_uvsphere_props = false;
						}

						ImGui::End();
					}
				}

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Plane", btn_size);

				if (ImGui::IsItemClicked())
					show_plane_props = true;

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
							Node* node = new Node();
							node->name = "Plane";
							Plane* plane = new Plane(plane_parameters.radius);
							plane->setMaterial(new PhongMaterial());

							node->transform.setRotation(glm::vec3(
								glm::radians(plane_parameters.rotation.x),
								glm::radians(plane_parameters.rotation.y),
								glm::radians(plane_parameters.rotation.z)
							));

							node->transform.setPosition(plane_parameters.position);
							node->meshes.push_back(plane);
							editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->addNode(node);
							show_plane_props = false;
						}

						ImGui::End();
					}
				}

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				primitives_opened = false;

			if (Utils::treeNode("Lights"))
			{
				lights_opened = true;
				ImGui::Dummy(ImVec2(0, 5.0f));

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Directional", btn_size);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Point", btn_size);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Spot", btn_size);

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
				ImGui::Button("First Person", btn_size);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY()));
				ImGui::Button("Third Person", btn_size);

				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}
			else
				cameras_opened = false;

			ImGui::End();
		}
	}

	void Tools::togglePanel()
	{
		panel_visible = !panel_visible;
	}

}