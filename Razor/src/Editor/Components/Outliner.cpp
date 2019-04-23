#include "rzpch.h"
#include "imgui.h"
#include "Outliner.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"

namespace Razor {

	Outliner::Outliner(Editor* editor) : EditorComponent(editor)
	{
		scenesManager = editor->getEngine()->getScenesManager();
	}

	Outliner::~Outliner()
	{
	}

	void Outliner::render(float delta)
	{
		ImGui::Begin("Outliner");

		ImGui::Text("Objects: %d", scenesManager->getActiveScene()->getNodesSize());

		ImGui::End();
	}

}