#include "rzpch.h"
#include <imgui.h>
#include "Editor/Editor.h"
#include "EditorView.h"
#include "Razor/Core/Engine.h"
#include "Razor/Rendering/Renderer.h"

namespace Razor
{

	EditorView::EditorView(Editor* editor) :
		EditorComponent(editor)
	{
		renderer = editor->getEngine()->getRenderer();
	}

	EditorView::~EditorView()
	{

	}

	void EditorView::render(float delta)
	{
		bool open = true;
		//ImGui::Begin("Viewport", &open);

		////TextureAttachment* color_buffer = renderer->getColorBuffer();

		//if (color_buffer != nullptr)
		//{
		//	auto win_size = ImGui::GetWindowSize();

		//	ImGui::SetCursorPos(ImVec2(0, 21.0f));
		//	//ImGui::Image((void*)(intptr_t)color_buffer->getId(), ImVec2(win_size.x, win_size.y - 21.0f));
		//}

		//ImGui::End();
	}

}
