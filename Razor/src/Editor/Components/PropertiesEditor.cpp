#include "rzpch.h"
#include "imgui.h"
#include "PropertiesEditor.h"
#include "Editor/Editor.h"

namespace Razor 
{

	PropertiesEditor::PropertiesEditor(Editor* editor) :
		EditorComponent(editor),
		selection(nullptr),
		position(glm::vec3(0.0f)),
		rotation(glm::vec3(0.0f)),
		scale(glm::vec3(1.0f)),
		transform_opened(true),
		static_mesh_opened(false),
		material_opened(false),
		name("test")
	{
		selection = (Selection*)editor->getTools()["selection"];
	}

	PropertiesEditor::~PropertiesEditor()
	{
	}

	void PropertiesEditor::render(float delta)
	{
		static PropertiesEditor app(this->editor);

		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;
		ImFont* font = atlas->Fonts[0];

		ImGui::Begin("Properties Editor");
		
		Node* selected = nullptr;

		if (selection->getNodes().size() > 0)
			selected = selection->getNodes().back();

		if (selected != nullptr)
		{
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##name", (char*)selected->name.c_str(), 32);
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::SetNextTreeNodeOpen(transform_opened);

			if (Utils::treeNode("Transform"))
			{
				transform_opened = true;
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				float col_w = 0.0f, input_size = 0.0f;
				float text_x_offset_y = 3.5f;

				// Position
				ImGui::Columns(2, "position_columns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 70.0f);
					initial_column_spacing++;
				}

				ImGui::Text("Position");
				ImGui::NextColumn();

				ImGui::PushItemWidth(-1.0f);
				ImGui::DragFloat3("##position", &selected->transform.getPosition()[0]);
				ImGui::PopItemWidth();

				// Rotation
				ImGui::NextColumn();
				ImGui::Text("Rotation");
				ImGui::NextColumn();

				ImGui::PushItemWidth(-1.0f);
				ImGui::DragFloat3("##rotation", &selected->transform.getRotation()[0]);
				ImGui::PopItemWidth();

				// Scale
				ImGui::NextColumn();
				ImGui::Text("Scale");
				ImGui::NextColumn();

				ImGui::PushItemWidth(-1.0f);
				ImGui::DragFloat3("##scale", &selected->transform.getScale()[0]);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				ImGui::TreePop();
			}
			else
				transform_opened = false;

			ImGui::SetNextTreeNodeOpen(static_mesh_opened);

			if (Utils::treeNode("Static Mesh"))
			{
				static_mesh_opened = true;
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (selected->meshes.size() > 0)
					ImGui::Text(selected->meshes[0]->getName().c_str());

				ImGui::TreePop();
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}
			else
				static_mesh_opened = false;

			ImGui::SetNextTreeNodeOpen(material_opened);

			if (Utils::treeNode("Material"))
			{
				material_opened = true;
	
				if (selected->meshes.size() > 0) 
				{
					if (selected->meshes[0]->getMaterial() != nullptr)
					{
						const char* class_name = typeid(*selected->meshes[0]->getMaterial()).name();
						std::string part = Utils::splitString(class_name, "::")[1];
						std::string str = part.substr(1, part.size());

						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::Columns(2, "material_cols", false);

						static unsigned short initial_column_spacing = 0;

						if (initial_column_spacing < 2)
						{
							ImGui::SetColumnWidth(0, 50.0f);
							initial_column_spacing++;
						}

						ImGui::Text("Type");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
							
						const char* items[] = { "ColorMaterial", "PbrMaterial", "PhongMaterial" };

						if (str == "PhongMaterial")
							current_material = "PhongMaterial";
						else if (str == "ColorMaterial")
							current_material = "ColorMaterial";
						else if (str == "PbrMaterial")
							current_material = "PbrMaterial";

						static ImGuiComboFlags flags = 0;

						if (ImGui::BeginCombo("combo 1", str.c_str(), flags))
						{
							for (int n = 0; n < IM_ARRAYSIZE(items); n++)
							{
								bool is_selected = (str.c_str() == items[n]);

								if (ImGui::Selectable(items[n], is_selected))
									selected->meshes[0]->getMaterial()->setName(items[n]);

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}

						ImGui::PopItemWidth();
						ImGui::Columns(1);
					}
				}

				ImGui::TreePop();
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}
			else
				material_opened = false;
		}

		ImGui::End();
	}

}