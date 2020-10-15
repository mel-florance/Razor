#pragma once

#include "Razor/Core/Core.h"

namespace Razor 
{
	class Editor;
	class Texture;

	class RAZOR_API FileBrowser
	{
	public:
		FileBrowser(Editor* editor);
		~FileBrowser();

		void render();



		struct TreeItem
		{
			TreeItem(const std::string& name, const std::string& path) :
				name(name),
				path(path),
				id(0)
			{}

			unsigned int id;
			std::string name;
			std::string path;
			//AssetsManager::Type type;
			std::vector<TreeItem*> childs;
			bool opened = false;

			inline void addChild(TreeItem* child)
			{
				auto it = std::find_if(childs.begin(), childs.end(), [=](TreeItem* i) {
					return i->name == child->name;
				});

				if (it == childs.end())
					childs.push_back(child);
			}
		};

		void loadTreeItem(TreeItem* item, const std::string& path, unsigned int depth);
		void unloadTreeItem(TreeItem* item);
		void drawTreeItem(TreeItem* item, unsigned int depth);
		static std::string getHomeDirectory();
		inline std::string getHomeProjectsDirectory() { return home_projects_directory; }
		inline static std::string getCurrentPath() { return current_path; }

	private:
		static std::string current_path;
		char filter_current[128];
		char filter[128];
		TreeItem* selected;
		unsigned int index;
		TreeItem* root;
		Texture* thumb_shadow;
		Editor* editor;
		std::string home_projects_directory;
		std::string projects_location_name;
	};

}
