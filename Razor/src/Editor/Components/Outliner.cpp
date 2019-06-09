#include "rzpch.h"
#include "imgui.h"
#include "Outliner.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Geometry/Geometry.h"

namespace Razor {

	Outliner::Outliner(Editor* editor) : 
		EditorComponent(editor)
	{
		scenesManager = editor->getEngine()->getScenesManager();
		selection = editor->getTool<Selection*>("selection");
	}

	Outliner::~Outliner()
	{
	}

	void Outliner::drawNode(std::shared_ptr<Node> node, unsigned int index)
	{
		if (node != nullptr)
		{
			ImGui::PushID(ImGui::GetID(std::to_string(node->id).c_str()));

			//if (node->nodes.size() == 0)
			//{
			//	if (node->meshes.size() == 1) {
			//		
			//		StaticMesh* mesh = node->meshes[0];

			//		if (dynamic_cast<UVSphere*>(mesh) != nullptr)
			//			editor->drawIcon("sphere", glm::vec2(16, 16));
			//		else if((dynamic_cast<Cube*>(mesh) != nullptr))
			//			editor->drawIcon("cube", glm::vec2(16, 16));
			//		else if ((dynamic_cast<Plane*>(mesh) != nullptr))
			//			editor->drawIcon("plane", glm::vec2(16, 16));
			//		
			//		ImGui::SameLine();
			//	}

			//	ImGui::Text(node->name.c_str());
			//}
			//else
			const char* name = node->name.c_str();
			if (Utils::treeNode(name, true, index % 2 == 0, "node", "node_hover", selection->isSelected(node)))
			{
				/*if (node->meshes.size() > 1)
			{
				for (auto& mesh : node->meshes)
					if (Utils::treeNode(mesh->getName().c_str()))
						ImGui::TreePop();
			}
			else if (node->meshes.size() == 1) {

				ImGui::Text(node->meshes[0]->getName().c_str());
				ImGui::Dummy(ImVec2(0, 5.0f));
			}
	*/

				std::vector<std::shared_ptr<Node>>::iterator it = node->nodes.begin();

				for (unsigned int i = 0; it != node->nodes.end(); ++it, ++i)
					drawNode(*it, i);

				ImGui::TreePop();
			}

			ImGui::PopID();

			if (ImGui::IsItemClicked())
			{
				selection->addNode(node);
			}
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

		ImGui::BeginChild("##Outliner_nodes", ImVec2(size.x, size.y - 56.0f));

		for (it = nodes.begin(); it != nodes.end(); ++it, ++i)
			drawNode(*it, i);

		ImGui::EndChild();

		ImVec2 screen_pos = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(screen_pos.x, screen_pos.y),
			ImVec2(screen_pos.x + size.x - 8.0f, screen_pos.y + 1.0f),
			ImColor(255, 255, 255, 30)
		);

		ImGui::Dummy({0, 7});
		ImGui::TextColored(ImColor(255, 255, 255, 128), "Objects %d", scenesManager->getActiveScene()->getNodesSize());

		ImGui::End();
	}

}