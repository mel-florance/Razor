#include "rzpch.h"
#include "imgui.h"
#include "FileBrowser.h"
#include "ImageEditor.h"
#include "AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Core/TasksManager.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Editor/Editor.h"
#include "Razor/Types/Color.h"

//#include "Razor/Assets/protos/asset.pb.h"

namespace fs = std::filesystem;

namespace Razor 
{
	std::string FileBrowser::current_path = "C:/";

	FileBrowser::FileBrowser(Editor* editor) :
		editor(editor),
		index(0),
		filter(" "),
		selected(nullptr),
		thumb_shadow(nullptr),
		filter_current(" "),

		home_projects_directory(""),
		projects_location_name("RazorProjects")
	{
		std::string home = getHomeDirectory();
		home_projects_directory =  home + "/" + projects_location_name;

		if (!fs::is_directory(home_projects_directory) || !fs::exists(home_projects_directory))
			fs::create_directory(home_projects_directory);

		current_path = home_projects_directory;

		root = new TreeItem("Razor", current_path);
		root->opened = true;
		root->id = index;

		loadTreeItem(root, current_path, 0);
		selected = root;

		thumb_shadow = new Texture("./data/thumb_shadow.png");
	}

	FileBrowser::~FileBrowser()
	{

	}

	void FileBrowser::render()
	{
		bool result = false;

		float child_width = 0.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 0.0f));
		ImGui::Columns(2, "twoColumns", true);

		static unsigned short initial_column_spacing = 0;
		if (initial_column_spacing < 2)
		{
			ImGui::SetColumnWidth(0, 220.0f);
			initial_column_spacing++;
		}

		ImGui::PushItemWidth(-1);

		static char str0[128] = "";
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 0.8f, 0.8f));
		ImGui::Button("IMPORT", ImVec2(ImGui::GetColumnWidth() - 8.0f, 22.5f));

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::Dummy(ImVec2(0, 4.0f));
		ImVec2 o = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(4.0f, ImGui::GetCursorPosY()));
		editor->icons_manager->drawIcon("magnify", glm::vec2(22.0f));
		ImGui::SetCursorPos(o);
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::SetCursorPos(ImVec2(4.0f, ImGui::GetCursorPosY()));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(29.0f, 4.0f));
		ImGui::InputTextWithHint("##search_dirs", "Search directories...", str0, IM_ARRAYSIZE(str0));
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Dummy(ImVec2(0, 4.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 4.0f));
		ImGui::BeginChild("Directories");
		drawTreeItem(root, 0);
		child_width = ImGui::GetItemRectMax().x;
		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::NextColumn();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4.0f, 4.0f));
		ImGui::PushItemWidth(-1);
		
		editor->icons_manager->drawIcon("arrow_previous", glm::vec2(20.0f), IconsManager::IconType::BUTTON);
		ImGui::SameLine();
		editor->icons_manager->drawIcon("arrow_next", glm::vec2(20.0f), IconsManager::IconType::BUTTON);
		std::string path = ("Razor" + current_path.substr(home_projects_directory.size(), current_path.size()));
		std::vector<std::string> parts = Utils::splitString(path, "/");
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(8.0f, 20.0f));
		unsigned int i = 0;

		for (auto p : parts) 
		{
			if (i > 0) {
				ImGui::SameLine();
				editor->icons_manager->drawIcon("arrow_right", glm::vec2(20.0f));
			}

			ImGui::SameLine();
			ImVec2 old = ImGui::GetFont()->DisplayOffset;
			ImGui::GetFont()->DisplayOffset.y = 2.0f;
			ImGui::Text(p.c_str());
			ImGui::GetFont()->DisplayOffset = old;

			++i;
		}

		ImGui::Indent(-4.0f);
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::Dummy(ImVec2(0, 6.0f));

		ImGui::Indent(8.0f);
		editor->icons_manager->drawIcon("magnify", glm::vec2(21.0f));
		ImGui::Indent(-8.0f);
		ImGui::SameLine();

		ImVec2 old = ImGui::GetFont()->DisplayOffset;
		ImGui::PushItemWidth(-1);
		static char str1[128] = "";
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25.0f, 4.0f));
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 21.0f, ImGui::GetCursorPosY()));
		ImGui::InputTextWithHint("##search_files", "Search files...", str0, IM_ARRAYSIZE(str1));
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();	
		ImGui::Dummy(ImVec2(0, 4.0f));
		
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		if (selected != nullptr) 
		{
			ImGui::BeginChild("Files");

			float col_width = 80.0f;
			float width = ImGui::GetWindowWidth() - col_width * 0.5f;
			int cols = (int)((width / col_width) + 0.5f);

			if (cols < 2)
				cols = 2;

			ImGui::Columns(cols, "filesColumns", false);
		
			int i = 0;
			for (const auto& entry : fs::directory_iterator(selected->path))
			{
				ImGui::SetColumnWidth(i % cols, col_width);

				if (!entry.is_directory()) 
				{
					bool btn_selected = false;

					AssetsManager* am = editor->getComponentsManager()->getComponent<AssetsManager>("AssetsManager");
					Texture* t = am->texturesManager->getTexture(entry.path().string());
					
					if (thumb_shadow != nullptr) {
						ImVec2 old = ImGui::GetCursorPos();
						ImGui::Image((void*)(intptr_t)thumb_shadow->getId(), ImVec2(70, 70));
				
						if (t != nullptr)
						{
							ImGui::SetCursorPosX(old.x + 5.0f);
							ImGui::SetCursorPosY(old.y + 5.0f);
							ImGui::Image((void*)(intptr_t)t->getId(), ImVec2(60, 60));
						}
					}
					else
						editor->icons_manager->drawIcon("file", glm::vec2(col_width));

					if(ImGui::IsItemClicked())
						btn_selected = true;

					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImGui::PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + col_width, pos.y + 30.0f), true);
					ImGui::Indent(10.0f);
					ImGui::TextWrapped(entry.path().filename().string().c_str());
					ImGui::Indent(-10.0f);
					ImGui::PopClipRect();

					if (ImGui::IsMouseDoubleClicked(0) && (ImGui::IsItemClicked() || btn_selected))
					{
						ImageEditor* ie = editor->getComponentsManager()->getComponent<ImageEditor>("ImageEditor");
						ie->loadImage(entry.path().string().c_str());
						//Node* node = nullptr;
						//AssetsManager::import(node, &Editor::importFinished, Variant(entry.path().string()));
					}
				
					ImGui::NextColumn();
				}

				i++;
			}
			
			ImGui::EndChild();
			ImGui::Columns(1);
		}

		ImGui::Indent(20.0f);
		ImGui::Columns(1);
	}

	void FileBrowser::loadTreeItem(TreeItem* item, const std::string& path, unsigned int depth)
	{
		for (const auto& entry : fs::directory_iterator(path)) 
		{
			if (entry.is_directory())
			{
				std::string dir_name = entry.path().filename().string();
				TreeItem* child = new TreeItem(dir_name, path + "/" + dir_name);
				child->id = ++index;
				depth++;
				loadTreeItem(child, path + "/" + dir_name, depth);
				item->addChild(child);
			}
		}
	}

	void FileBrowser::unloadTreeItem(TreeItem* item)
	{
		std::vector<TreeItem*>::iterator it = item->childs.begin();

		for(; it != item->childs.end(); ++it)
			delete *it;

		item->childs.clear();
		item->opened = false;
	}

	void FileBrowser::drawTreeItem(TreeItem* item, unsigned int depth)
	{
		if (item != nullptr)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 4));

			float line_height = 20.0f;

			ImVec2 old = ImGui::GetFont()->DisplayOffset;

			float offset_x = item->childs.size() > 0 
				? line_height * 3.2f + line_height * depth 
				: (line_height * depth) + line_height * 2.2f;

			offset_x -= line_height;

			ImGui::GetFont()->DisplayOffset.x = offset_x;
			ImGui::GetFont()->DisplayOffset.y = 2.0f;
			ImGui::SetCursorPosX(0.0f);
			ImGui::Selectable(
				item->name.c_str(), 
				item == selected,
				ImGuiSelectableFlags_AllowItemOverlap, 
				ImVec2(ImGui::GetWindowWidth(), line_height)
			);
			ImGui::GetFont()->DisplayOffset = old;

			if (ImGui::IsItemClicked()) {

				if (ImGui::IsMouseDoubleClicked(0) && item->childs.size() > 0)
					item->opened = !item->opened;

				selected = item;

				if (item->path.size() > 0)
					current_path = item->path;
			}

			ImGui::SameLine();

			if (item->childs.size() > 0) 
			{
				ImGui::SetCursorPosX(depth * line_height);
				editor->icons_manager->drawIcon("folder", glm::vec2(line_height));
				ImGui::SameLine();
				const char* icon = item->opened ? "arrow_down" : "arrow_right";
				editor->icons_manager->drawIcon(icon, glm::vec2(line_height));
					
				if (ImGui::IsItemClicked())
					item->opened = !item->opened;
			}
			else {
				ImGui::SetCursorPosX(depth * line_height);
				editor->icons_manager->drawIcon(!item->opened ? "folder" : "arrow_down", glm::vec2(line_height));
			}
			
			if (item->opened)
			{ 
				std::vector<TreeItem*>::iterator it = item->childs.begin();
				depth++;

				ImGui::Indent(line_height);

				for (; it != item->childs.end(); ++it)
					drawTreeItem(*it, depth);

				ImGui::Indent(-line_height);
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
	}

	std::string FileBrowser::getHomeDirectory()
	{
		std::string str = std::string(getenv("USERPROFILE"));
		std::set<char> delims{ '\\' };
		auto strings = Utils::splitString(str, "\\");

		return Utils::joinStrings(strings, "/");
	}

}
