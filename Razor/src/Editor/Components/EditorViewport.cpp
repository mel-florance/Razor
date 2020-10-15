#include "rzpch.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include "EditorViewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Core/Utils.h"
#include "Razor/Core/Viewport.h"
#include "Razor/Rendering/Renderer.h"

#include "Razor/ImGui/ImGuizmo.h"
#include "imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Editor/Tools/GridAxis.h"
#include "Editor/Tools/Gizmo.h"
#include "Editor/Tools/Selection.h"
#include <glad/glad.h>

#include "Razor/Rendering/BillboardManager.h"

namespace Razor
{

	EditorViewport::EditorViewport(Editor* editor) :
		EditorComponent(editor),
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
		is_fullscreen(false),
		current_pass(0),
		show_fps(false)
	{
		viewport = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera()->getViewport();

		renderer = editor->getEngine()->getRenderer();
		g_buffer = renderer->getGBuffer();
		current_pass = g_buffer->getCombined();

		//ForwardRenderer* renderer = Application::Get().getForwardRenderer();
		grid_axis = (GridAxis*)editor->getToolsManager()->getTool<GridAxis>("grid_axis");
		gizmo = editor->getToolsManager()->getTool<Gizmo>("gizmo");
		selection = editor->getToolsManager()->getTool<Selection>("selection");

		/*if (grid_axis != nullptr)
		{
			renderer->addLineMesh(grid_axis->getAxisNode(), 0);
			renderer->addLineMesh(grid_axis->getGridNode(), 1);
		}*/
	}

	EditorViewport::~EditorViewport()
	{

	}

	void EditorViewport::render(float delta)
	{
		int flags = ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::Begin("Viewport", &viewport->isVisible(), flags);

	

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Properties"))
			{
				if (ImGui::MenuItem("Show FPS", "Ctrl + F", show_fps))
					show_fps = !show_fps;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View type"))
			{
				ImGui::MenuItem("Front");
				if (ImGui::IsItemClicked()) {}
				ImGui::MenuItem("Back");
				if (ImGui::IsItemClicked()) {}
				ImGui::MenuItem("Left");
				if (ImGui::IsItemClicked()) {}
				ImGui::MenuItem("Right");
				if (ImGui::IsItemClicked()) {}
				ImGui::MenuItem("Bottom");
				if (ImGui::IsItemClicked()) {}
				ImGui::MenuItem("Top");
				if (ImGui::IsItemClicked()) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View mode"))
			{
				if (ImGui::MenuItem("Combined", "F1", current_pass == g_buffer->getCombined()))
					current_pass = g_buffer->getCombined();
				if (ImGui::MenuItem("Position", "F2", current_pass == g_buffer->getPosition()))
					current_pass = g_buffer->getPosition();
				if (ImGui::MenuItem("Normal", "F3", current_pass == g_buffer->getNormal()))
					current_pass = g_buffer->getNormal();
				if (ImGui::MenuItem("Color", "F4", current_pass == g_buffer->getColor()))
					current_pass = g_buffer->getColor();
				if (ImGui::MenuItem("Splitted", "F5", current_pass == 999))
					current_pass = 999;

				ImGui::EndMenu();
			}

		

			ImGui::EndMenuBar();
		}

		auto screen_pos = ImGui::GetCursorScreenPos();
		auto win_size = ImGui::GetWindowSize();
		auto win_pos_x = ImGui::GetCursorPosX();
		auto win_pos_y = ImGui::GetCursorPosY();


		size = glm::vec2(win_size.x, win_size.y);
		ImGui::Dummy(ImVec2(size.x, size.y - 21.0f));
		auto rect_pos = ImGui::GetItemRectMin();
		auto rect_max = ImGui::GetItemRectMax();
		auto rect_size = ImVec2(rect_pos.x + 150.0f, rect_pos.y + (35.0f + 90.0f));

		Camera* cam = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera();

		ImGui::SetCursorPos(ImVec2(0, 21.0f));
		float y_margin = 21.0f;

		if (current_pass != 999) {
			ImGui::Image((void*)(intptr_t)current_pass, ImVec2(size.x, size.y - y_margin));

			cam->getViewport()->setHovered(ImGui::IsItemHovered());
		}
		else
		{
			float x = size.x / 2.0f;
			float y = (size.y - y_margin) / 2.0f;
			bool hovered = false;

			// Combined
			ImVec2 old_combined_pos = ImGui::GetCursorScreenPos();
			ImVec2 start_combined_pos = ImGui::GetCursorPos();
			ImGui::Image((void*)(intptr_t)g_buffer->getCombined(), ImVec2(x, y));
			hovered = ImGui::IsItemHovered();
			ImVec2 combined_pos = ImGui::GetCursorPos();

			ImVec2 old_position_pos = ImGui::GetCursorScreenPos();
			ImVec2 start_position_pos = ImGui::GetCursorPos();

			ImGui::SetCursorScreenPos(ImVec2(old_combined_pos.x + 15.0f, old_combined_pos.y + 35.0f));
			ImGui::Text("Combined");
			ImGui::SetCursorPos(ImVec2(combined_pos.x, combined_pos.y - 35.0f));

			// Position
			ImGui::SetCursorPos(ImVec2(x + 1.0f, start_combined_pos.y));
			ImGui::Image((void*)(intptr_t)g_buffer->getPosition(), ImVec2(x, y));
			hovered = ImGui::IsItemHovered();
			ImVec2 position_pos = ImGui::GetCursorPos();

			ImGui::SetCursorScreenPos(ImVec2(old_position_pos.x + 15.0f, old_position_pos.y + 35.0f));
			ImGui::Text("Position");
			ImGui::SetCursorPos(ImVec2(position_pos.x, position_pos.y - 35.0f));

			// Normal
			ImVec2 pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y - 1.0f));
			ImGui::Image((void*)(intptr_t)g_buffer->getNormal(), ImVec2(x, y));
			hovered = ImGui::IsItemHovered();

			ImGui::SameLine(0.0f, 1.0f);

			// Color
			ImGui::Image((void*)(intptr_t)g_buffer->getColor(), ImVec2(x, y));
			hovered = ImGui::IsItemHovered();

			viewport->setHovered(hovered);
		}

		if (show_fps)
		{
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

			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 0.7f });

			float y = 70.0f;
			float x = 25.0f;
			ImGui::SetCursorPos(ImVec2(x, y));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "FPS");
			ImGui::SetCursorPos(ImVec2(x + 60.0f, y));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "%.f", editor->getEngine()->getFPS());

			ImGui::SetCursorPos(ImVec2(x, y + 20.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Frame");
			ImGui::SetCursorPos(ImVec2(x + 60, y + 20.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f", editor->getEngine()->getFrameTiming());

			ImGui::SetCursorPos(ImVec2(x, y + 40.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Update");
			ImGui::SetCursorPos(ImVec2(x + 60, y + 40.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f", editor->getEngine()->getUpdateTiming());

			ImGui::SetCursorPos(ImVec2(x, y + 60.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Render");
			ImGui::SetCursorPos(ImVec2(x + 60, y + 60.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f", editor->getEngine()->getRenderTiming());

			ImGui::SetCursorPos(ImVec2(x, y + 80.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "Sleep");
			ImGui::SetCursorPos(ImVec2(x + 60, y + 80.0f));
			ImGui::TextColored(ImColor(255, 255, 255, 128), "%.3f", editor->getEngine()->getSleepTiming());

			ImGui::PopStyleColor();
		}

		std::shared_ptr<Node> selected = nullptr;

		if (selection->getNodes().size() > 0)
			selected = selection->getNodes().back();


		if (selected != nullptr)
		{
			Camera* cam = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera();
			const char* cam_class = typeid(*cam).name();
			std::string part = Utils::splitString(cam_class, "::")[1];
			std::string cam_type = part.substr(1, part.size());

			gizmo->setNodePtr(selected);
			gizmo->setCamera(cam);
			gizmo->setViewportPosition(glm::vec2(screen_pos.x, screen_pos.y));
			gizmo->setViewportSize(glm::vec2(size.x, size.y + 21.0f));

			if (Input::IsKeyPressed(RZ_KEY_KP_DECIMAL) && isHovered())
			{
				if (cam_type == "TPSCamera")
				{
					cam->setHasTarget(true);
					((TPSCamera*)cam)->setTarget(&selected->transform);
					float ratio = (selected->transform.getScale().x + selected->transform.getScale().y + selected->transform.getScale().z) / 3.0f;
					((TPSCamera*)cam)->setDistance(PI * ratio);
				}
			}

			if (Input::IsKeyPressed(RZ_KEY_G) && isHovered())
				gizmo->setCurrentOperation(ImGuizmo::OPERATION::TRANSLATE);

			if (Input::IsKeyPressed(RZ_KEY_R) && isHovered())
				gizmo->setCurrentOperation(ImGuizmo::OPERATION::ROTATE);

			if (Input::IsKeyPressed(RZ_KEY_Z) && isHovered())
				gizmo->setCurrentOperation(ImGuizmo::OPERATION::SCALE);

			gizmo->update(delta);
		}


		/*ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(ForwardRenderer::unprojected_position.x + 145, ForwardRenderer::unprojected_position.y - 120),
			ImVec2(ForwardRenderer::unprojected_position.x + 240, ForwardRenderer::unprojected_position.y),
			IM_COL32(255, 30, 30, 128)
		);*/

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
					/*if (renderer->hasLineMesh(g_node))
						renderer->removeLineMesh(g_node);
					else
						renderer->addLineMesh(g_node, 1);*/
				}


				ImGui::Checkbox("Show axis", &show_grid_axis);

				if (ImGui::IsItemClicked())
				{
					/*std::shared_ptr<Node> node = grid_axis->getAxisNode();

					if (renderer->hasLineMesh(node))
						renderer->removeLineMesh(node);
					else
						renderer->addLineMesh(node, 0);*/
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
				ImGui::DragFloat("##viewport_speed", &cam->getSpeed(), 1.0f, cam->getMinSpeed(), cam->getMaxSpeed(), "%.2f");
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


		ImGui::End();
	}

	void EditorViewport::onEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed) {
			KeyPressedEvent& e = (KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_F1)
				current_pass = g_buffer->getCombined();
			else if (e.GetKeyCode() == RZ_KEY_F2)
				current_pass = g_buffer->getPosition();
			else if (e.GetKeyCode() == RZ_KEY_F3)
				current_pass = g_buffer->getNormal();
			else if (e.GetKeyCode() == RZ_KEY_F4)
				current_pass = g_buffer->getColor();
			else if (e.GetKeyCode() == RZ_KEY_F5)
				current_pass = 999;

			if (e.GetKeyCode() == RZ_KEY_F && (e.GetMods() & GLFW_MOD_CONTROL)) {
				show_fps = !show_fps;
			}
		}
	}

	bool EditorViewport::isHovered()
	{
		return viewport->isHovered();
	}

	void EditorViewport::toggleFullscreen()
	{
		is_fullscreen = !is_fullscreen;
	}

	void EditorViewport::toggleProperties()
	{
		show_properties = !show_properties;
	}

	void EditorViewport::showContextMenu()
	{
		ImVec2 mouse_pos = ImGui::GetMousePos();
		context_menu_position = glm::vec2(mouse_pos.x, mouse_pos.y);
		show_context_menu = true;
	}

}