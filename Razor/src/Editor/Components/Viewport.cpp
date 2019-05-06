#include "rzpch.h"
#include "imgui.h"
#include "Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Core/Utils.h"

#include "Razor/ImGui/ImGuizmo.h"
#include "imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Editor/Tools/GridAxis.h"
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
		grid_size(10),
		viewport_clip_start(0.001f),
		viewport_clip_end(1000.0f),
		cursor_position(glm::vec3(0.0f)),
		is_fullscreen(false)
	{

		DeferredRenderer* renderer = Application::Get().getDeferredRenderer();
		grid_axis = (GridAxis*)editor->getTools()["grid_axis"];

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

		//ImGui::SetNextWindowPos(ImVec2(0, 0));
		//ImGui::SetNextWindowSize(ImVec2(1920, 1080));

		ImGui::Begin("Viewport");

		auto win_size = ImGui::GetWindowSize();
		auto win_pos_x = ImGui::GetCursorPosX();
		auto win_pos_y = ImGui::GetCursorPosY();

		size = glm::vec2(win_size.x, win_size.y);

		DeferredRenderer* renderer = Application::Get().getDeferredRenderer();
		auto cam = editor->getEngine()->getScenesManager()->getActiveScene()->getActiveCamera();

		if (renderer != nullptr)
		{
			auto buffer = renderer->getColorBuffer();
			auto tex = buffer->getId();
			
			ImGui::Image((void*)(intptr_t)tex, ImVec2(size.x, size.y - 22.0f));
		
			auto rect_pos = ImGui::GetItemRectMin();
			auto rect_max = ImGui::GetItemRectMax();
			auto rect_size = ImVec2(rect_pos.x + 150.0f, rect_pos.y + (35.0f + 80.0f));

			static float snap[3] = { 1.f, 1.f, 1.f };
			static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
			static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
			static bool boundSizing = false;
			static bool boundSizingSnap = false;
			static bool useSnap = true;


			
			//ImGuizmo::DrawCube(.
			//	&cam->getViewMatrix()[0][0],
			//	&cam->getProjectionMatrix()[0][0], 
			//	&transform.getMatrix()[0][0]
			//);

			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(rect_pos.x, rect_pos.y, size.x, size.y - 22.0f);

			Node* node = editor->getEngine()->getScenesManager()->getActiveScene()->getSceneGraph()->getNodes()[1];
			glm::mat4 proj = glm::perspectiveFov(cam->getFov(), size.x, size.y, cam->getClipNear(), cam->getClipFar());
			glm::mat4 view = cam->getViewMatrix();

			view[0][1] = -view[0][1];
			view[1][1] = -view[1][1];
			view[2][1] = -view[2][1];
			
			glm::mat4 v = view;
			glm::mat4 p = proj;
			glm::mat4 object = node->transform.getMatrix();

			ImGuizmo::Manipulate(
				glm::value_ptr(view),
				glm::value_ptr(proj),
				ImGuizmo::TRANSLATE, 
				ImGuizmo::LOCAL, 
				&object[0][0]
			);

			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(&object[0][0], matrixTranslation, matrixRotation, matrixScale);

			node->transform.setPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
			node->transform.setRotation(glm::vec3(glm::radians(matrixRotation[0]), glm::radians(matrixRotation[1]), glm::radians(matrixRotation[2])));
			node->transform.setScale(glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]));



			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(rect_pos.x + 8.0f, rect_pos.y + 8.0f),
				rect_size,
				IM_COL32(30, 30, 30, 128)
			);

			ImGui::GetWindowDrawList()->AddRect(
				ImVec2(rect_pos.x + 8.0f, rect_pos.y + 8.0f),
				rect_size,
				IM_COL32(60, 60, 60, 128)
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
					ImGui::Checkbox("Show grid", &show_grid);

					Node* g_node = grid_axis->getGridNode();

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
						Node* node = grid_axis->getAxisNode();

						if (renderer->hasLineMesh(node))
							renderer->removeLineMesh(node);
						else
							renderer->addLineMesh(node, 0);
					}

					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("Size  ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);

					Grid* grid_mesh = (Grid*)g_node->meshes[0];

					ImGui::DragFloat("", &grid_mesh->getDivisions(), 1.0f, 0.0f, 25.0f); // FIX ME: Bug in the grid algo generation above 25.

					if (ImGui::IsItemEdited() && grid_size > 0)
					{
						Node* node = grid_axis->getGridNode();
						Grid grid = Grid(grid_mesh->getDivisions());

						node->meshes[0]->getVbo()->update((unsigned int)grid.getVertices().size() * sizeof(float), &grid.getVertices()[0]);
					}

					ImGui::PopItemWidth();

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

					ImGui::Text("X ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##cursor_position_x", &cursor_position.x);
					ImGui::PopItemWidth();

					ImGui::Text("Y ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##cursor_position_y", &cursor_position.y);
					ImGui::PopItemWidth();

					ImGui::Text("Z ");
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

					ImGui::Text("Speed ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_speed", &cam->getSpeed(), 1.0f,  cam->getMinSpeed(), cam->getMaxSpeed(), "%.2f");
					ImGui::PopItemWidth();

					ImGui::Text("Fov   ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_fov", &cam->getFov(), 1.0f, 1.0f, 179.0f, "%.2f");
					ImGui::PopItemWidth();

					ImGui::Text("Near  ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_clip_near", &cam->getClipNear());
					ImGui::PopItemWidth();

					ImGui::Text("Far   ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 85.0f);
					ImGui::DragFloat("##viewport_clip_far", &cam->getClipFar());
					ImGui::PopItemWidth();
		
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Text("Mode");
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					int e = cam->getMode() == Camera::Mode::ORTHOGRAPHIC ? 1 : 0;
					ImGui::RadioButton("Perspective ", &e, 0);

					if (ImGui::IsItemClicked())
						cam->setMode(Camera::Mode::PERSPECTIVE);

					ImGui::RadioButton("Orthographic", &e, 1);

					if (ImGui::IsItemClicked())
						cam->setMode(Camera::Mode::ORTHOGRAPHIC);

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Text("Position");
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Text("X ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##viewport_camera_position_x", &cam->getPositionX());
					ImGui::PopItemWidth();

					ImGui::Text("Y ");
					ImGui::SameLine();
					ImGui::PushItemWidth(col_width - 57.0f);
					ImGui::DragFloat("##viewport_camera_position_y", &cam->getPositionY());
					ImGui::PopItemWidth();

					ImGui::Text("Z ");
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
			ImGui::Text("FPS    %f", editor->getEngine()->getFPS());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 20.0f));
			ImGui::Text("Frame  %f", editor->getEngine()->getFrameTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 40.0f));
			ImGui::Text("Update %f", editor->getEngine()->getUpdateTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 60.0f));
			ImGui::Text("Render %f", editor->getEngine()->getRenderTiming());
			ImGui::SetCursorPos(ImVec2(15.0f, y + 80.0f));
			ImGui::Text("Sleep  %f", editor->getEngine()->getSleepTiming());

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

}