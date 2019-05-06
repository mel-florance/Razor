#include "rzpch.h"
#include "imgui.h"
#include "Outliner.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"

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

	void Outliner::drawNode(Node* node)
	{
		ImGui::PushID(ImGui::GetID(std::to_string(node->id).c_str()));

		if (node->nodes.size() == 0)
			ImGui::Text(node->name.c_str());
		else if (Utils::treeNode(node->name.c_str()))
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
			for (auto child : node->nodes)
				drawNode(child);

			ImGui::TreePop();
		}

		ImGui::PopID();

		if (ImGui::IsItemClicked())
		{
			selection->addNode(node);
		}
	}

	void Outliner::render(float delta)
	{
		ImGui::Begin("Outliner");
		
		std::shared_ptr<Scene> scene = editor->getEngine()->getScenesManager()->getActiveScene();
		
		for (auto node : scene->getSceneGraph()->getNodes())
			drawNode(node);

		//ImGui::Text("Objects: %d", scenesManager->getActiveScene()->getNodesSize());

		ImGui::End();
	}

}