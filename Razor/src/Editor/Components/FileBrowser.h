#pragma once

#include "Razor/Core/Core.h"

namespace Razor 
{

	class RAZOR_API FileBrowser
	{
	public:
		FileBrowser();
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

		void loadTreeItem(TreeItem* item, const std::string& path);
		void unloadTreeItem(TreeItem* item);
		void drawTreeItem(TreeItem* item);

	private:
		TreeItem* selected;
		unsigned int index;
		TreeItem* root;
		char filter[32];
	};

}
