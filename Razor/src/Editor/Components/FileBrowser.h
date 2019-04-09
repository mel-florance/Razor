#pragma once

#include "imgui.h"
#include "Razor/Core/Core.h"

namespace fs = std::experimental::filesystem;

namespace Razor {

	class TasksManager;
	class AssetsManager;

	class RAZOR_API FileBrowser
	{
	public:
		FileBrowser();
		~FileBrowser();

		const struct File {
			std::string name;
			fs::path path;
		};

		void render();
		static void getFiles(const fs::path& path, std::vector<File>& files);
		static const int clampToInt(const size_t data);
		static bool getFromVector(void* data, int idx, const char** out_text);
		static void itemClicked(int id);

		static std::vector<File> filesInScope;
		static TasksManager* tasksManager;
		static AssetsManager* assetsManager;

	private:
		int selection;
		std::string filepath;
		bool oldVisibility;
		bool isVisible;
		fs::path currentPath;
		bool currentPathIsDir;
	};

}
