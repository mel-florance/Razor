#include "rzpch.h"
#include "imgui.h"
#include "AddShape.h"
#include "Editor/Editor.h"

namespace Razor
{

	AddShape::AddShape(Editor* editor) :
		EditorModal(editor),
		current_shape(ShapeType::CIRCLE)
	{
	}

	AddShape::~AddShape()
	{
	}

	void AddShape::init()
	{

	}

	void AddShape::render()
	{
		if (active)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			int flags = ImGuiWindowFlags_NoDocking  | 
						ImGuiWindowFlags_NoCollapse |
						ImGuiWindowFlags_NoResize;

			if (ImGui::Begin(currentShapeToStr().c_str(), &active, flags))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);
				Utils::initColumns(100.f);
			}
		}
	}
}
