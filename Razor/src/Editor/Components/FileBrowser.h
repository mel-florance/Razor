#pragma once

#include "imgui.h"
#include "Razor/Core.h"
#include "Editor/EditorComponent.h"

namespace fs = std::experimental::filesystem;

namespace Razor {

	class RAZOR_API FileBrowser : public EditorComponent
	{
	public:
		FileBrowser(Editor* editor);
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

	private:
		int selection;
		std::string filepath;
		bool oldVisibility;
		bool isVisible;
		fs::path currentPath;
		bool currentPathIsDir;
	};

}
