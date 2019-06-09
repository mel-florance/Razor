#include "rzpch.h"
#include "imgui.h"
#include "PropertiesEditor.h"
#include "Editor/Editor.h"
#include "Razor/Core/Engine.h"
#include "Razor/Scene/Scene.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Components/AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Lighting/Spot.h"
#include <Commdlg.h>

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
		lights_opened(true),
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
		
		std::shared_ptr<Node> selected = nullptr;

		if (selection->getNodes().size() > 0)
			selected = selection->getNodes().back();

		if (selected != nullptr)
		{
			ImGui::PushItemWidth(-1);
			
			ImGui::InputText("##name", &selected->name[0u], selected->name.size() + 128);
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

				if (selected->meshes.size() > 0)
				{
					ImGui::TextColored(ImColor(255, 255, 255, 128), "Position");
					ImGui::NextColumn();

					ImGui::PushItemWidth(-1.0f);
					ImGui::DragFloat3("##position", &selected->transform.getPosition()[0], 0.01f);
					ImGui::PopItemWidth();

					// Rotation
					ImGui::NextColumn();
					ImGui::TextColored(ImColor(255, 255, 255, 128), "Rotation");
					ImGui::NextColumn();

					ImGui::PushItemWidth(-1.0f);
					ImGui::DragFloat3("##rotation", &selected->transform.getRotation()[0], 0.01f, 0.0f, PI);
					ImGui::PopItemWidth();

					// Scale
					ImGui::NextColumn();
					ImGui::TextColored(ImColor(255, 255, 255, 128), "Scale");
					ImGui::NextColumn();

					ImGui::PushItemWidth(-1.0f);
					ImGui::DragFloat3("##scale", &selected->transform.getScale()[0], 0.01f, 0.001f, 99999999999999.0f);
					ImGui::PopItemWidth();
			
				}
				else if (selected->lights.size() > 0)
				{
					Light::Type type = selected->lights[0]->getType();

					if (type == Light::Type::DIRECTIONAL)
					{
						std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(selected->lights[0]);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Direction");
						ImGui::NextColumn();

						ImGui::PushItemWidth(-1.0f);
						ImGui::DragFloat3("##direction", &directional->getDirection()[0], 0.01f);
						ImGui::PopItemWidth();
					}
					else if (type == Light::Type::POINT)
					{
						std::shared_ptr<Point> point = std::dynamic_pointer_cast<Point>(selected->lights[0]);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Position");
						ImGui::NextColumn();

						ImGui::PushItemWidth(-1.0f);
						ImGui::DragFloat3("##position", &point->getPosition()[0], 0.01f);
						ImGui::PopItemWidth();
					}
					else if (type == Light::Type::SPOT)
					{
						std::shared_ptr<Spot> spot = std::dynamic_pointer_cast<Spot>(selected->lights[0]);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Position");
						ImGui::NextColumn();

						ImGui::PushItemWidth(-1.0f);
						ImGui::DragFloat3("##position", &spot->getPosition()[0], 0.01f);
						ImGui::PopItemWidth();

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Direction");
						ImGui::NextColumn();

						ImGui::PushItemWidth(-1.0f);
						ImGui::DragFloat3("##direction", &spot->getDirection()[0], 0.01f);
						ImGui::PopItemWidth();
					}
				}

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				ImGui::TreePop();
			}
			else
				transform_opened = false;

			ImGui::SetNextTreeNodeOpen(static_mesh_opened);

			if (selected->meshes.size() > 0)
			{
				if (Utils::treeNode("Static Mesh"))
				{
					static_mesh_opened = true;

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Indent(-21.0f);
					ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

					if (ImGui::TreeNode("Display"))
					{
						ImGui::Indent(5.0f);
						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::Columns(2, "mesh_cols", true);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Show bounding box");
						ImGui::NextColumn();
						ImGui::Checkbox("##show_bounding_box", &selected->meshes[0]->isBoundingBoxVisible());
						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Backface culling");
						ImGui::NextColumn();
						ImGui::Checkbox("##backface_culling", &selected->meshes[0]->hasCulling());
						ImGui::NextColumn();

						static unsigned short initial_column_spacing = 0;

						if (initial_column_spacing < 2)
						{
							ImGui::SetColumnWidth(0, 130.0f);
							initial_column_spacing++;
						}

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Winding order");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						std::array winding_orders = StaticMesh::getWindingOrdersStr();
						const char* current_winding_order = selected->meshes[0]->getWindingOrderStr();

						if (ImGui::BeginCombo("##winding_orders", current_winding_order, 0))
						{
							for (int n = 0; n < winding_orders.size(); n++)
							{
								bool is_selected = (current_winding_order == winding_orders[n]);

								if (ImGui::Selectable(winding_orders[n], is_selected))
									selected->meshes[0]->setWindingOrder(winding_orders[n]);

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}

						/////

						ImGui::PopItemWidth();
						ImGui::NextColumn();
						ImGui::TextColored(ImColor(255, 255, 255, 128), "Culling type");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						std::array cull_types = StaticMesh::getCullTypesStr();
						const char* current_cull_type = selected->meshes[0]->getCullTypeStr();

						if (ImGui::BeginCombo("##cull_types", current_cull_type, 0))
						{
							for (int n = 0; n < cull_types.size(); n++)
							{
								bool is_selected = (current_cull_type == cull_types[n]);

								if (ImGui::Selectable(cull_types[n], is_selected))
									selected->meshes[0]->setCullType(cull_types[n]);

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}

						/////

						ImGui::PopItemWidth();
						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Draw mode");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						std::array draw_modes = StaticMesh::getDrawModesStr();
						const char* current_draw_mode = selected->meshes[0]->getDrawModeStr();

						if (ImGui::BeginCombo("##draw_modes", current_draw_mode, 0))
						{
							for (int n = 0; n < draw_modes.size(); n++)
							{
								bool is_selected = (current_draw_mode == draw_modes[n]);

								if (ImGui::Selectable(draw_modes[n], is_selected))
									selected->meshes[0]->setDrawMode(draw_modes[n]);

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}

						ImGui::PopItemWidth();
						ImGui::NextColumn();

						switch (selected->meshes[0]->getDrawMode()) 
						{
						case StaticMesh::DrawMode::LINES:
						case StaticMesh::DrawMode::LINE_LOOP:
						case StaticMesh::DrawMode::LINE_STRIP:

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Dashed lines");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::Checkbox("##draw_mode_line_dashed", &selected->meshes[0]->isLineDashed());
							ImGui::PopItemWidth();
							ImGui::NextColumn();

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Lines width");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat("##draw_mode_line_width", &selected->meshes[0]->getLineWidth(), 1.0f, 0, 10);
							ImGui::PopItemWidth();
							ImGui::NextColumn();

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Line factor");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragInt("##draw_mode_line_factor", &selected->meshes[0]->getLineFactor(), 1.0f, 0);
							ImGui::PopItemWidth();
							ImGui::NextColumn();

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Line pattern");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragInt("##draw_mode_line_pattern", &selected->meshes[0]->getLinePattern(), 1.0f, 0);
							ImGui::PopItemWidth();
							ImGui::NextColumn();
						}

						ImGui::Columns(1);

						ImGui::Indent(-5.0f);
						ImGui::TreePop();
						ImGui::Dummy(ImVec2(0.0f, 5.0f));
					}

					ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
					if (ImGui::TreeNode("Infos"))
					{
						ImGui::Indent(5.0f);
						ImGui::Dummy(ImVec2(0.0f, 5.0f));

						if (selected->meshes.size() > 0)
						{
							ImGui::Columns(2, "mesh_infos", true);

							static unsigned short initial_column_spacing = 0;
							if (initial_column_spacing < 2)
							{
								ImGui::SetColumnWidth(0, 130.0f);
								initial_column_spacing++;
							}

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Name");
							ImGui::NextColumn();
							ImGui::Text(selected->meshes[0]->getName().c_str());

							ImGui::NextColumn();

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Vertex count");
							ImGui::NextColumn();
							ImGui::Text((Utils::numberFormatLocale(selected->meshes[0]->getVertexCount())).c_str());

							ImGui::NextColumn();

							ImGui::TextColored(ImColor(255, 255, 255, 128), "Triangles count");
							ImGui::NextColumn();
							ImGui::Text((Utils::numberFormatLocale(selected->meshes[0]->getIndices().size() / 3)).c_str());

							ImGui::Columns(1);
						}

						ImGui::Indent(-5.0f);
						ImGui::TreePop();
						ImGui::Dummy(ImVec2(0.0f, 5.0f));
					}

					ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
					if (ImGui::TreeNode("Physics"))
					{
						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::Indent(5.0f);
						ImGui::Columns(2, "position_columns", true);

						static unsigned short initial_column_spacing = 0;
						if (initial_column_spacing < 2)
						{
							ImGui::SetColumnWidth(0, 130.0f);
							initial_column_spacing++;
						}

						ImGui::Text("Enable");
						ImGui::NextColumn();
						ImGui::Checkbox("##checkbox_physics_enabled", &selected->meshes[0]->getPhysicsEnabled());

						ImGui::Columns(1);
						ImGui::Indent(-5.0f);
						ImGui::TreePop();
					}

					ImGui::Indent(21.0f);
					ImGui::TreePop();
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
				}
				else
					static_mesh_opened = false;
			}
			

			if (selected->meshes.size() > 0)
			{
				ImGui::SetNextTreeNodeOpen(material_opened);

				if (Utils::treeNode("Material"))
				{
					material_opened = true;

					if (selected->meshes[0]->getMaterial() != nullptr)
					{
						const char* class_name = typeid(*selected->meshes[0]->getMaterial()).name();
						std::string part = Utils::splitString(class_name, "::")[1];
						std::string str = part.substr(1, part.size());

						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::Columns(2, "material_cols", true);

						static unsigned short initial_column_spacing = 0;

						if (initial_column_spacing < 2)
						{
							ImGui::SetColumnWidth(0, 130.0f);
							initial_column_spacing++;
						}

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Lighting model");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						const char* items[] = { "Color", "PBR", "Phong", "Landscape" };

						if (str == "PhongMaterial")
							current_material = "Phong";
						else if (str == "ColorMaterial")
							current_material = "Color";
						else if (str == "PbrMaterial")
							current_material = "PBR";
						else if (str == "LandscapeMaterial")
							current_material = "Landscape";

						static ImGuiComboFlags flags = 0;

						if (ImGui::BeginCombo("##materials_list", current_material, flags))
						{
							for (int n = 0; n < IM_ARRAYSIZE(items); n++)
							{
								bool is_selected = strcmp(current_material, items[n]) == 0;

								if (ImGui::Selectable(items[n], is_selected))
									selected->meshes[0]->getMaterial()->setName(items[n]);

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}

						ImGui::PopItemWidth();
						ImGui::NextColumn();

						static bool alpha_preview = true;
						static bool alpha_half_preview = true;
						static bool drag_and_drop = true;
						static bool options_menu = true;
						static bool hdr = true;
						ImColor textColor = ImColor(255, 255, 255, 128);

						int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0)
							| (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop)
							| (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0))
							| (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

						PhongMaterial* mat = (PhongMaterial*)selected->meshes[0]->getMaterial().get();

						ImGui::TextColored(textColor, "Alpha");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ImGui::DragFloat("##Alpha", &mat->getAlpha(), 1.0f, 0.0f, 1.0f);
						ImGui::PopItemWidth();

						ImGui::Columns(1);

						ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Diffuse"))
						{
							ImGui::Dummy(ImVec2(0.0f, 5.0f));
							ImGui::Indent(5.0f);
							ImGui::Columns(2, "diffuse_columns", true);

							static unsigned short initial_column_spacing = 0;
							if (initial_column_spacing < 2)
							{
								ImGui::SetColumnWidth(0, 130.0f);
								initial_column_spacing++;
							}

							ImGui::TextColored(textColor, "Color");
							ImGui::NextColumn();
							ImGui::ColorEdit3("##pickerDiffuse", &mat->getDiffuseColor()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Texture");
							ImGui::NextColumn();

							bool isDiffusePathEmpty = (selected->meshes[0]->getMaterial()->getDiffusePath() == "Not set");
							ImGui::Button(selected->meshes[0]->getMaterial()->getDiffusePath().c_str(), ImVec2(ImGui::GetColumnWidth() - (11.0f + (!isDiffusePathEmpty ? 21.0f : 0.0f)), 21.0f));

							if (ImGui::IsItemHovered() && !isDiffusePathEmpty)
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted(selected->meshes[0]->getMaterial()->getDiffusePath().c_str());
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}

							if (ImGui::IsItemClicked())
							{
								std::string str = Utils::fileDialog();

								if (str.size() > 0)
								{

									selected->meshes[0]->getMaterial()->setDiffusePath(str);
									Texture* texture = new Texture(str, true);
									AssetsManager::texturesManager->addTexture(str, texture);
									selected->meshes[0]->getMaterial()->setTextureMap(Material::TextureType::Diffuse, texture->getId());
								}
							}

							if (!isDiffusePathEmpty)
							{
								ImGui::SameLine();
								ImGui::Button("X", ImVec2(21.0f, 21.0f));

								if (ImGui::IsItemClicked())
								{
									selected->meshes[0]->getMaterial()->removeTextureMap(Material::TextureType::Diffuse);
									selected->meshes[0]->getMaterial()->setDiffusePath("Not set");
									AssetsManager::texturesManager->removeTexture(selected->meshes[0]->getMaterial()->getDiffusePath());
								}

								ImGui::NextColumn();
								ImGui::NextColumn();
								ImGui::Image((void*)(intptr_t)selected->meshes[0]->getMaterial()->getDiffuseMap(), ImVec2(80.f, 80.f));
							}

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Tiling");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat2("##DiffuseTiling", &mat->getDiffuseTiling()[0], 1.0f, 0.0f);
							ImGui::PopItemWidth();

							ImGui::Columns(1);
							ImGui::Indent(-5.0f);
							ImGui::TreePop();
						}

						ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Specular"))
						{
							ImGui::Dummy(ImVec2(0.0f, 5.0f));
							ImGui::Indent(5.0f);
							ImGui::Columns(2, "specular_columns", true);

							static unsigned short initial_column_spacing = 0;
							if (initial_column_spacing < 2)
							{
								ImGui::SetColumnWidth(0, 130.0f);
								initial_column_spacing++;
							}

							ImGui::TextColored(textColor, "Color");
							ImGui::NextColumn();
							ImGui::ColorEdit3("##pickerSpecular", &mat->getSpecularColor()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Texture");
							ImGui::NextColumn();

							bool isSpecularPathEmpty = (selected->meshes[0]->getMaterial()->getSpecularPath() == "Not set");
							ImGui::Button(selected->meshes[0]->getMaterial()->getSpecularPath().c_str(), ImVec2(ImGui::GetColumnWidth() - (11.0f + (!isSpecularPathEmpty ? 21.0f : 0.0f)), 21.0f));

							if (ImGui::IsItemHovered() && !isSpecularPathEmpty)
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted(selected->meshes[0]->getMaterial()->getSpecularPath().c_str());
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}

							if (ImGui::IsItemClicked())
							{
								std::string str = Utils::fileDialog();

								if (str.size() > 0)
								{
									Texture* texture = new Texture(str, true);
									AssetsManager::texturesManager->addTexture(str, texture);
									selected->meshes[0]->getMaterial()->setTextureMap(Material::TextureType::Specular, texture->getId());
									selected->meshes[0]->getMaterial()->setSpecularPath(str);
								}
							}

							if (!isSpecularPathEmpty)
							{
								ImGui::SameLine();
								ImGui::Button("X", ImVec2(21.0f, 21.0f));

								if (ImGui::IsItemClicked())
								{
									selected->meshes[0]->getMaterial()->removeTextureMap(Material::TextureType::Specular);
									selected->meshes[0]->getMaterial()->setSpecularPath("Not set");
									AssetsManager::texturesManager->removeTexture(selected->meshes[0]->getMaterial()->getSpecularPath());
								}

								ImGui::NextColumn();
								ImGui::NextColumn();
								ImGui::Image((void*)(intptr_t)selected->meshes[0]->getMaterial()->getSpecularMap(), ImVec2(80.f, 80.f));
							}

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Shininess");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat("##Shininess", &mat->getShininess(), 0.01f);
							ImGui::PopItemWidth();

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Strength");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat("##ShininessStrength", &mat->getShininessStrength(), 0.01f);
							ImGui::PopItemWidth();

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Tiling");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat2("##SpecularTiling", &mat->getSpecularTiling()[0], 0.1f);
							ImGui::PopItemWidth();

							ImGui::Columns(1);
							ImGui::Indent(-5.0f);
							ImGui::TreePop();
						}

						ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Ambient"))
						{
							ImGui::Dummy(ImVec2(0.0f, 5.0f));
							ImGui::Indent(5.0f);
							ImGui::Columns(2, "ambient_columns", true);

							static unsigned short initial_column_spacing = 0;
							if (initial_column_spacing < 2)
							{
								ImGui::SetColumnWidth(0, 130.0f);
								initial_column_spacing++;
							}

							ImGui::TextColored(textColor, "Color");
							ImGui::NextColumn();
							ImGui::ColorEdit3("##pickerAmbient", &mat->getAmbientColor()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

							ImGui::Columns(1);
							ImGui::Indent(-5.0f);
							ImGui::TreePop();
						}

						ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Normal"))
						{
							ImGui::Dummy(ImVec2(0.0f, 5.0f));
							ImGui::Indent(5.0f);
							ImGui::Columns(2, "normal_columns", true);

							static unsigned short initial_column_spacing = 0;
							if (initial_column_spacing < 2)
							{
								ImGui::SetColumnWidth(0, 130.0f);
								initial_column_spacing++;
							}

							ImGui::TextColored(textColor, "Texture");
							ImGui::NextColumn();

							bool isNormalPathEmpty = (selected->meshes[0]->getMaterial()->getNormalPath() == "Not set");
							ImGui::Button(selected->meshes[0]->getMaterial()->getNormalPath().c_str(), ImVec2(ImGui::GetColumnWidth() - (11.0f + (!isNormalPathEmpty ? 21.0f : 0.0f)), 21.0f));

							if (ImGui::IsItemHovered() && !isNormalPathEmpty)
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								ImGui::TextUnformatted(selected->meshes[0]->getMaterial()->getNormalPath().c_str());
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}

							if (ImGui::IsItemClicked())
							{
								std::string str = Utils::fileDialog();

								if (str.size() > 0)
								{
									Texture* texture = new Texture(str, true);
									AssetsManager::texturesManager->addTexture(str, texture);
									selected->meshes[0]->getMaterial()->setTextureMap(Material::TextureType::Normal, texture->getId());
									selected->meshes[0]->getMaterial()->setNormalPath(str);
								}
							}

							if (!isNormalPathEmpty)
							{
								ImGui::SameLine();
								ImGui::Button("X", ImVec2(21.0f, 21.0f));

								if (ImGui::IsItemClicked())
								{
									selected->meshes[0]->getMaterial()->removeTextureMap(Material::TextureType::Normal);
									selected->meshes[0]->getMaterial()->setNormalPath("Not set");
									AssetsManager::texturesManager->removeTexture(selected->meshes[0]->getMaterial()->getNormalPath());
								}

								ImGui::NextColumn();
								ImGui::NextColumn();
								ImGui::Image((void*)(intptr_t)selected->meshes[0]->getMaterial()->getNormalMap(), ImVec2(80.f, 80.f));
							}

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Strength");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat("##NormalStrength", &mat->getNormalStrength(), 0.1f);
							ImGui::PopItemWidth();

							ImGui::NextColumn();

							ImGui::TextColored(textColor, "Tiling");
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							ImGui::DragFloat2("##NormalTiling", &mat->getNormalTiling()[0], 1.0f, 0.0f);
							ImGui::PopItemWidth();

							ImGui::Columns(1);
							ImGui::Indent(-5.0f);
							ImGui::TreePop();
						}

						ImGui::Columns(1);
					}

					ImGui::TreePop();
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
				}
				else
					material_opened = false;
			}

			if (selected->lights.size() > 0)
			{
				ImGui::SetNextTreeNodeOpen(lights_opened);

				if (Utils::treeNode("Light"))
				{
					lights_opened = true;

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Indent(5.0f);
					ImGui::Columns(2, "ambient_columns", true);

					static unsigned short initial_column_spacing = 0;
					if (initial_column_spacing < 2)
					{
						ImGui::SetColumnWidth(0, 130.0f);
						initial_column_spacing++;
					}

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Type");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					const char* light_class_name = typeid(*selected->lights[0]).name();
					std::string part = Utils::splitString(light_class_name, "::")[1];
					std::string str = part.substr(1, part.size());

					const char* items[] = { "Directional", "Point", "Spot" };

					current_light_type = str.c_str();

					static ImGuiComboFlags flags = 0;

					if (ImGui::BeginCombo("##light_type_list", current_light_type, flags))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							bool is_selected = strcmp(current_light_type, items[n]) == 0;

							if (ImGui::Selectable(items[n], is_selected))
							{
								if (items[n] == "Directional")
								{
									editor->getEngine()->getScenesManager()->getActiveScene()->removeLight(selected->lights[0]);
									std::shared_ptr<Directional> directional = std::make_shared<Directional>();
									selected->lights[0] = directional;
									str = "Directional";
									editor->getEngine()->getScenesManager()->getActiveScene()->addLight(directional, Light::Type::DIRECTIONAL);
								}
								else if (items[n] == "Point")
								{
									editor->getEngine()->getScenesManager()->getActiveScene()->removeLight(selected->lights[0]);
									std::shared_ptr<Point> point = std::make_shared<Point>();
									selected->lights[0] = point;
									str = "Point";
									editor->getEngine()->getScenesManager()->getActiveScene()->addLight(point, Light::Type::POINT);
								}
								else if (items[n] == "Spot")
								{
									editor->getEngine()->getScenesManager()->getActiveScene()->removeLight(selected->lights[0]);
									std::shared_ptr<Spot> spot = std::make_shared<Spot>();
									selected->lights[0] = spot;
									str = "Spot";
									editor->getEngine()->getScenesManager()->getActiveScene()->addLight(spot, Light::Type::SPOT);
								}		
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					ImGui::PopItemWidth();
					ImGui::NextColumn();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Cast shadows");
					ImGui::NextColumn();
					ImGui::Checkbox("##light_cast_shadow", &selected->lights[0]->isCastingShadows());

					ImGui::NextColumn();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Intensity");
					ImGui::NextColumn();
					ImGui::DragFloat("##light_intensity", &selected->lights[0]->getIntensity(), 0.1f);

					ImGui::NextColumn();

					static bool alpha_preview = true;
					static bool alpha_half_preview = true;
					static bool drag_and_drop = true;
					static bool options_menu = true;
					static bool hdr = true;
					ImColor textColor = ImColor(255, 255, 255, 128);

					int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0)
						| (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop)
						| (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0))
						| (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);


					ImGui::TextColored(ImColor(255, 255, 255, 128), "Color");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##pickerLightDiffuse", &selected->lights[0]->getDiffuse()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

					ImGui::NextColumn();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Specular");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##pickerLightSpecular", &selected->lights[0]->getSpecular()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

					ImGui::NextColumn();

					ImGui::TextColored(ImColor(255, 255, 255, 128), "Ambient");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##pickerLightAmbient", &selected->lights[0]->getAmbient()[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

					ImGui::NextColumn();

					if (str == "Directional")
					{

					}
					else if (str == "Point")
					{
						std::shared_ptr<Point> point = std::dynamic_pointer_cast<Point>(selected->lights[0]);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Constant");
						ImGui::NextColumn();
						ImGui::DragFloat("##PointLightConstant", &point->getConstant(), 0.1f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Linear");
						ImGui::NextColumn();
						ImGui::DragFloat("##PointLightLinear", &point->getLinear(), 0.01f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Quadratic");
						ImGui::NextColumn();
						ImGui::DragFloat("##PointLightQuadratic", &point->getQuadratic(), 0.001f);
					}
					else if (str == "Spot")
					{
						std::shared_ptr<Spot> spot = std::dynamic_pointer_cast<Spot>(selected->lights[0]);

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Constant");
						ImGui::NextColumn();
						ImGui::DragFloat("##SpotLightConstant", &spot->getConstant(), 0.1f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Linear");
						ImGui::NextColumn();
						ImGui::DragFloat("##SpotLightLinear", &spot->getLinear(), 0.01f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Quadratic");
						ImGui::NextColumn();
						ImGui::DragFloat("##SpotLightQuadratic", &spot->getQuadratic(), 0.001f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Inner cutoff");
						ImGui::NextColumn();
						ImGui::DragFloat("##SpotLightInnerCutoff", &spot->getInnerCutoff(), 0.1f);

						ImGui::NextColumn();

						ImGui::TextColored(ImColor(255, 255, 255, 128), "Outer cutoff");
						ImGui::NextColumn();
						ImGui::DragFloat("##SpotLightOuterCutoff", &spot->getOuterCutoff(), 0.1f);

						ImGui::NextColumn();
					}


					ImGui::Columns(1);
					ImGui::Indent(-5.0f);

					ImGui::TreePop();
				}
				else
					lights_opened = false;
			}
		}

		ImGui::End();
	}

}