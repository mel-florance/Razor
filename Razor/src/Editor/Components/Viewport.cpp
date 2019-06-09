#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Core/Utils.h"

#include "Razor/ImGui/ImGuizmo.h"
#include "imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Editor/Tools/GridAxis.h"
#include "Editor/Tools/Gizmo.h"
#include "Editor/Tools/Selection.h"
#include <glad/glad.h>

namespace Razor
{

	Viewport::Viewport(Editor* editor) :
		EditorComponent(editor),
		hovered(false),
		show_properties(false),
		grid_opened(true),
		cursor_opened(true),
		view_opened(true),
		show_grid(true),
		show_grid_axis(true),
		show_context_menu(false),
		context_menu_position(glm::vec2(0.0f)),
		grid_size(10),
		viewport_clip_start(0.001f),
		viewport_clip_end(1000.0f),
		cursor_position(glm::vec3(0.0f)),
		is_fullscreen(false)
	{

		ForwardRenderer* renderer = Application::Get().getForwardRenderer();
		grid_axis = (GridAxis*)editor->getTools()["grid_axis"];
		gizmo = editor->getTool<Gizmo*>("gizmo");
		selection = editor->getTool<Selection*>("selection");

		if (grid_axis != nullptr)
		{
			renderer->addLineMesh(grid_axis->getAxisNode(), 0);
			renderer->addLineMesh(grid_axis->getGridNode(), 1);
		}
	}

	Viewport::~Viewport()
	{
	}

	void Viewport::render(float delta)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); 

		bool show_viewport = true;
		ImGui::Begin("Viewport", &show_viewport, ImGuiWindowFlags_NoScrollbar);

		auto win_size = ImGui::GetWindowSize();
		auto win_pos_x = ImGui::GetCursorPosX();
		auto win_pos_y = ImGui::GetCursorPosY();

		size = glm::vec2(win_size.x, win_size.y);

		ForwardRenderer* renderer = Application::Get().getForwardRenderer();
		Camera* cam = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera();
		const char* cam_class = typeid(*cam).name();
		std::string part = Utils::splitString(cam_class, "::")[1];
		std::string cam_type = part.substr(1, part.size());

		ImGui::Dummy(ImVec2(size.x, size.y - 22.0f));
		auto rect_pos = ImGui::GetItemRectMin();
		auto rect_max = ImGui::GetItemRectMax();
		auto rect_size = ImVec2(rect_pos.x + 150.0f, rect_pos.y + (35.0f + 80.0f));

		hovered = ImGui::IsItemHovered();

		if (renderer != nullptr)
		{
			auto colorbuffer = renderer->getColorBuffer();
			auto depthbuffer = renderer->getDepthBuffer();

			auto color_tex = colorbuffer->getId();
			auto depth_tex = depthbuffer->getId();
			
			ImGui::SetCursorPos(ImVec2(0, 22.0f));
			ImGui::Image((void*)(intptr_t)color_tex, ImVec2(size.x, size.y - 22.0f));
	/*		ImGui::SetCursorPos(ImVec2(size.x / 2, 22.0f));
			ImGui::Image((void*)(intptr_t)depth_tex, ImVec2(size.x / 2, size.y - 22.0f));*/

			if (show_context_menu && ImGui::IsItemClicked()) {
				show_context_menu = false;
			}

			std::shared_ptr<Node> selected = nullptr;

			if (selection->getNodes().size() > 0)
				selected = selection->getNodes().back();

			if (selected != nullptr)
			{
				gizmo->setNodePtr(selected);
				gizmo->setCamera(cam);
				gizmo->setViewportPosition(glm::vec2(rect_pos.x, rect_pos.y));
				gizmo->setViewportSize(glm::vec2(size.x, size.y));

				if (Input::IsKeyPressed(RZ_KEY_KP_DECIMAL) && hovered)
				{
					cam->setHasTarget(true);

					if (cam_type == "TPSCamera") {
						((TPSCamera*)cam)->setTarget(&selected->transform);
						((TPSCamera*)cam)->setDistance(5.0f);
					}
				}

				if (Input::IsKeyPressed(RZ_KEY_G) && hovered)
					gizmo->setCurrentOperation(ImGuizmo::OPERATION::TRANSLATE);
				if (Input::IsKeyPressed(RZ_KEY_R) && hovered)
					gizmo->setCurrentOperation(ImGuizmo::OPERATION::ROTATE);
				if (Input::IsKeyPressed(RZ_KEY_Z) && hovered)
					gizmo->setCurrentOperation(ImGuizmo::OPERATION::SCALE);

				gizmo->update();
			}


			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(rect_pos.x + 8.0f, rect_pos.y + 8.0f),
				rect_size,
				IM_COL32(30, 30, 30, 128)
			);

			ImGui::GetWindowDrawList()->AddRect(
				ImVec2(rect_pos.x + 8.0f, rect_pos.y + 8.0f),
				rect_size,
				IM_COL32(255, 255, 255, 30)
			);

			if (show_properties)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				auto btn_size = ImVec2(160.0f, 25.0f);
				ImGui::SetNextTreeNodeOpen(grid_opened);
			
				ImGui::Begin("Properties");

				ImGui::Columns(2, "props_col", false);
				float col_width = ImGui::GetColumnWidth() * 2.0f;
				ImGui::NextColumn();
				ImGui::Columns(1);

				if (Utils::treeNode("Grid"))
				{
					grid_opened = true;

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5f));
					ImGui::Checkbox("Show grid", &show_grid);
		

					std::shared_ptr<Node> g_node = grid_axis->getGridNode();

					if (ImGui::IsItemClicked())
					{
						if (renderer->hasLineMesh(g_node))
							renderer->removeLineMesh(g_node);
						else
							renderer->addLineMesh(g_node, 1);
					}


					ImGui::Checkbox("Show axis", &show_grid_axis);

					if (ImGui::IsItemClicked())
					{
						std::shared_ptr<Node> node = grid_axis->getAxisNode();

						if (renderer->hasLineMesh(node))
							renderer->removeLineMesh(node);
						else
							renderer->addLineMesh(node, 0);
					}

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Size  ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);

					std::shared_ptr<Grid> grid_mesh = std::dynamic_pointer_cast<Grid>(g_node->meshes[0]);

					ImGui::DragInt("", &grid_mesh->getDivisions(), 1.0f, 1, 1024);

					if (ImGui::IsItemEdited() && grid_size > 0)
					{
						Grid grid(grid_mesh->getDivisions());
						grid_mesh->getVbo()->update((unsigned int)grid.getVertices().size() * sizeof(float), &grid.getVertices()[0]);

						GridAxis::Axis axis(grid_mesh->getDivisions());
						std::shared_ptr<Node> node = grid_axis->getAxisNode();
						node->meshes[0]->getVbo()->update((unsigned int)axis.x_ray->getVertices().size() * sizeof(float), &axis.x_ray->getVertices()[0]);
						node->meshes[1]->getVbo()->update((unsigned int)axis.y_ray->getVertices().size() * sizeof(float), &axis.y_ray->getVertices()[0]);
						node->meshes[2]->getVbo()->update((unsigned int)axis.z_ray->getVertices().size() * sizeof(float), &axis.z_ray->getVertices()[0]);
					}

					ImGui::PopItemWidth();

					ImGui::PopStyleColor();
					ImGui::TreePop();
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
				}
				else
					grid_opened = false;

				ImGui::SetNextTreeNodeOpen(cursor_opened);

				if (Utils::treeNode("Cursor"))
				{
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					cursor_opened = true;

					ImGui::TextColored(ImColor(255, 255, 255, 128), "X ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##cursor_position_x", &cursor_position.x);
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Y ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##cursor_position_y", &cursor_position.y);
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Z ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##cursor_position_z", &cursor_position.z);
					ImGui::PopItemWidth();

					ImGui::TreePop();
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
				}
				else
					cursor_opened = false;

				ImGui::SetNextTreeNodeOpen(view_opened);
				ImGui::PushItemWidth(-1.0f);

				if (Utils::treeNode("View"))
				{
					view_opened = true;
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Speed ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_speed", &cam->getSpeed(), 1.0f,  cam->getMinSpeed(), cam->getMaxSpeed(), "%.2f");
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Fov   ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_fov", &cam->getFov(), 1.0f, 1.0f, 179.0f, "%.2f");
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Near  ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_clip_near", &cam->getClipNear());
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Far   ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_clip_far", &cam->getClipFar());
					ImGui::PopItemWidth();
		
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Text("Mode");
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5f));

					int e = cam->getMode() == Camera::Mode::ORTHOGRAPHIC ? 1 : 0;
					ImGui::RadioButton("Perspective ", &e, 0);

					if (ImGui::IsItemClicked())
						cam->setMode(Camera::Mode::PERSPECTIVE);

					ImGui::RadioButton("Orthographic", &e, 1);

					ImGui::PopStyleColor();

					if (ImGui::IsItemClicked())
						cam->setMode(Camera::Mode::ORTHOGRAPHIC);

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Text("Position");
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::TextColored(ImColor(255, 255, 255, 128), "X ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##viewport_camera_position_x", &cam->getPositionX());
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Y ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##viewport_camera_position_y", &cam->getPositionY());
					ImGui::PopItemWidth();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Z ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##viewport_camera_position_z", &cam->getPositionZ());
					ImGui::PopItemWidth();

					ImGui::TreePop();
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
				}
				else
					view_opened = false;

				ImGui::PopItemWidth();

				ImGui::End();
			}
			
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 0.7f });
			
			float y = 35.0f;
			ImGui::SetCursorPos(ImVec2(15.0f, y));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "FPS    %f", editor->getEngine()->getFPS());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 20.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Frame  %f", editor->getEngine()->getFrameTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 40.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Update %f", editor->getEngine()->getUpdateTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 60.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Render %f", editor->getEngine()->getRenderTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 80.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Sleep  %f", editor->getEngine()->getSleepTiming());

			if (cam != nullptr)
			{
				float rangeA[] = { cam->getMinSpeed(), cam->getMaxSpeed() };
				float rangeB[] = { 1, 10 };
				float speed = Utils::mapValues(rangeA, rangeB, cam->getSpeed());

				char str[128];
				sprintf(str, "Speed %.1f", speed);
				ImVec2 margin = { 15.0f, 22.0f };
				ImVec2 text_size = ImGui::CalcTextSize(str);

				ImGui::SetCursorPos({
					size.x - text_size.x - margin.x,
					win_pos_x + text_size.y + margin.y 
				});

				ImGui::Text(str);
			}

			ImGui::PopStyleColor();
		}

		ImVec2 window_position = ImGui::GetWindowPos();

		if (show_context_menu)
		{
			ImGui::SetNextWindowPos(ImVec2(context_menu_position.x - 50, context_menu_position.y - 35));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 8));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4, 4));

			ImGui::BeginChild("contextMenu", ImVec2(100, 200), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_ChildWindow);

			ImGui::Text("Add");
			ImGui::Dummy(ImVec2(0, 2));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 2));

			if (ImGui::BeginMenu("Mesh"))
			{
				ImGui::MenuItem("Cube");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
					editor->getComponent<Tools*>("Tools")->showCubeProps();
				}

				ImGui::MenuItem("Plane");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
					editor->getComponent<Tools*>("Tools")->showPlaneProps();
				}

				ImGui::MenuItem("UV Sphere");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
					editor->getComponent<Tools*>("Tools")->showUVSphereProps();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light"))
			{
				ImGui::MenuItem("Directional");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
				}

				ImGui::MenuItem("Point");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
				}

				ImGui::MenuItem("Spot");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Camera"))
			{
				ImGui::MenuItem("Free");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
				}

				ImGui::MenuItem("Orbital");
				if (ImGui::IsItemClicked()) {
					show_context_menu = false;
				}

				ImGui::EndMenu();
			}

			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::SetNextWindowPos(window_position);
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Viewport::toggleFullscreen()
	{
	}

	void Viewport::toggleProperties()
	{
		show_properties = !show_properties;
	}

	void Viewport::showContextMenu()
	{
		ImVec2 mouse_pos = ImGui::GetMousePos();
		context_menu_position = glm::vec2(mouse_pos.x, mouse_pos.y);
		show_context_menu = true;
	}

}