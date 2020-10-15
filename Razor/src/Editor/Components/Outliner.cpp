#include "rzpch.h"
#include "imgui.h"
#include "Outliner.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Editor/Tools/Selection.h"
#include "Razor/Core/Engine.h"
#include "Razor/Geometry/Geometry.h"

namespace Razor 
{

	Outliner::Outliner(Editor* editor) :
		EditorComponent(editor),
		filtered_name(""),
		focused_node(nullptr)
	{
		scenesManager = editor->getEngine()->getScenesManager();
		selection = editor->getToolsManager()->getTool<Selection>("selection");
	}

	Outliner::~Outliner()
	{
	}

	void Outliner::drawNode(std::shared_ptr<Node> node, unsigned int index, unsigned int depth)
	{
		if (node != nullptr && selection != nullptr)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4, 4));

			const char* name = node->name.c_str();
			bool selected = selection->isSelected(node);

			float line_height = 20.0f;

			ImVec2 old = ImGui::GetFont()->DisplayOffset;
			ImGui::GetFont()->DisplayOffset.x = (line_height * 2.2f) + line_height * depth;
			ImGui::GetFont()->DisplayOffset.y = 2.0f;

			ImGui::SetCursorPosX(0.0f);
			ImGui::Selectable(name, selection->isSelected(node), ImGuiSelectableFlags_AllowItemOverlap, ImVec2(ImGui::GetWindowWidth(), line_height));
			ImGui::GetFont()->DisplayOffset = old;

			bool active = selection->isSelected(node) || ImGui::IsItemHovered();
	
			if (ImGui::IsItemClicked())
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

			if (ImGui::IsItemFocused())
				focused_node = node;

			//active = node->opened && ImGui::IsItemHovered();
		/*	if (node->meshes.size() > 1)
			{
				for (auto& mesh : node->meshes)
					if (Utils::treeNode(mesh->getName().c_str()))
						ImGui::TreePop();
			}*/
			/*else if (node->meshes.size() == 1)
			{
				std::shared_ptr<StaticMesh> mesh = node->meshes[0];

				if (std::dynamic_pointer_cast<UVSphere>(mesh) != nullptr)
					editor->drawIcon("sphere", glm::vec2(16, 16));
				else if ((std::dynamic_pointer_cast<Cube>(mesh) != nullptr))
					editor->drawIcon("cube", glm::vec2(16, 16));
				else if ((std::dynamic_pointer_cast<Plane>(mesh) != nullptr))
					editor->drawIcon("plane", glm::vec2(16, 16));

				ImGui::SameLine();

				ImGui::TextColored(ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.8f)), node->meshes[0]->getName().c_str());
				ImGui::Dummy(ImVec2(0, 5.0f));
			}*/

			std::vector<std::shared_ptr<Node>>::iterator it = node->nodes.begin();

			ImGui::SameLine();
			ImGui::SetCursorPosX(0.0f);
			editor->icons_manager->drawIcon(node->active ? "eye_open" : "eye_closed", glm::vec2(line_height));

			if (ImGui::IsItemClicked())
				node->active = !node->active;

			ImGui::SameLine();
			ImGui::SetCursorPosX(depth * line_height + line_height);

			const char* icon =
				node->nodes.size() == 0
				? !selection->isSelected(node) ? "node" : "node_hover"
				: (!node->opened ? "arrow_right" : "arrow_down");

			editor->icons_manager->drawIcon(icon, glm::vec2(line_height));

			if (ImGui::IsItemClicked())
				node->opened = !node->opened;

			if (node->opened)
			{
				ImGui::Indent(line_height);
				depth++;

				for (unsigned int i = 0; it != node->nodes.end(); ++it, ++i)
					drawNode(*it, i, depth);

				ImGui::Indent(-line_height);
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
	}

	void Outliner::render(float delta)
	{
		ImGui::Begin("Outliner");
		
		std::shared_ptr<Scene> scene = editor->getEngine()->getScenesManager()->getActiveScene();
		std::vector<std::shared_ptr<Node>> nodes = scene->getSceneGraph()->getNodes();
		std::vector<std::shared_ptr<Node>>::iterator it;
		unsigned int i = 0;

		ImVec2 size = ImGui::GetWindowSize();

		editor->icons_manager->drawIcon("magnify", glm::vec2(22.0f));
		ImGui::SameLine();

		ImGui::PushItemWidth(-1);
		ImGui::Indent(-1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25.0f, 4.0f));
		ImGui::InputTextWithHint("##search_scene", "Search objects...", &filtered_name[0u], filtered_name.size() + 128);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();

		ImGui::BeginChild("##Outliner_nodes", ImVec2(size.x, size.y - 88.0f));

		for (it = nodes.begin(); it != nodes.end(); ++it, ++i)
			drawNode(*it, i, 0);

		ImGui::EndChild();

		ImVec2 screen_pos = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(screen_pos.x, screen_pos.y),
			ImVec2(screen_pos.x + size.x - 8.0f, screen_pos.y + 1.0f),
			ImColor(255, 255, 255, 30)
		);

		ImGui::Dummy({0, 7});
		ImGui::TextColored(ImColor(255, 255, 255, 128), "Objects: %d", scenesManager->getActiveScene()->getNodesSize());
		ImGui::SameLine();
		ImGui::TextColored(ImColor(255, 255, 255, 128), "Instances: %d", scenesManager->getActiveScene()->getInstancesSize());

		ImGui::End();
	}

	void Outliner::onEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed) {
			KeyPressedEvent& e = (KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_ENTER && focused_node != nullptr)
			{
				if (focused_node->nodes.size() == 0)
				{
					if (selection->isSelected(focused_node))
						selection->removeNode(focused_node->id);
					else
					{
						selection->clear();
						selection->addNode(focused_node);
					}
				}
				else
					focused_node->opened = !focused_node->opened;
			}
		}
	}

}