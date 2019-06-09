#include "rzpch.h"
#include "imgui.h"
#include "FileBrowser.h"
#include "AssetsManager.h"
#include "Razor/Core/TasksManager.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Editor/Editor.h"

namespace fs = std::filesystem;

namespace Razor 
{

	FileBrowser::FileBrowser() :
		index(0),
		filter(""),
		selected(nullptr)
	{
		std::string path = "F:/Razor/Razor";
		root = new TreeItem("Razor", path);
		root->opened = false;
		root->id = index;
		loadTreeItem(root, path);
	}

	FileBrowser::~FileBrowser()
	{
	}

	void FileBrowser::render()
	{
		bool result = false;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::Columns(2, "twoColumns", true);
		ImGui::PopStyleVar();

		static unsigned short initial_column_spacing = 0;
		if (initial_column_spacing < 2)
		{
			ImGui::SetColumnWidth(0, 220.0f);
			initial_column_spacing++;
		}

		ImGui::PushItemWidth(-1);
		ImGui::InputText("Search", filter, 32);
		ImGui::PopItemWidth();

		ImGui::BeginChild("Directories");
		drawTreeItem(root);
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("Files");

		if (selected != nullptr) 
		{
			ImGui::Columns(8, "filesColumns", false);

			for (const auto& entry : fs::directory_iterator(selected->path))
			{
				if (!entry.is_directory()) 
				{
					ImGui::Button("##zer", ImVec2(ImGui::GetColumnWidth() / 1.333f, ImGui::GetColumnWidth() / 1.333f));

					if (ImGui::Selectable(entry.path().filename().string().c_str())) {
						Log::trace(entry.path().string().c_str());
					}

					ImGui::Dummy(ImVec2(0, 20));
					ImGui::NextColumn();
				}
			}

			ImGui::Columns(1);
		}

		ImGui::EndChild();

		ImGui::Columns(1);
	}

	void FileBrowser::loadTreeItem(TreeItem* item, const std::string& path)
	{
		for (const auto& entry : fs::directory_iterator(path)) 
		{
			if (entry.is_directory())
			{
				std::string dir_name = entry.path().filename().string();
				TreeItem* child = new TreeItem(dir_name, path + "/" + dir_name);
				child->id = ++index;
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
	}

	void FileBrowser::drawTreeItem(TreeItem* item)
	{
		if (item != nullptr)
		{
			ImGui::SetNextTreeNodeOpen(item->opened, ImGuiCond_Once);
			
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4, 4));

			bool node_opened = ImGui::TreeNode(item, item->name.c_str());

			if(node_opened)
			{
				item->opened = true;
				std::vector<TreeItem*>::iterator it = item->childs.begin();

				for (; it != item->childs.end(); ++it)
					drawTreeItem(*it);

				ImGui::TreePop();
			}
			else
				item->opened = false;

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();

			if (ImGui::IsItemClicked() && node_opened)
				unloadTreeItem(item);
			else if(ImGui::IsItemClicked() && !node_opened)
				loadTreeItem(item, item->path);

			if (ImGui::IsItemClicked())
				selected = item;
		}
	}

}
