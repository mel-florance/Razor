#include "rzpch.h"
#include "imgui.h"
#include "FileBrowser.h"
#include "AssetsManager.h"
#include "Razor/Core/TasksManager.h"
#include "Razor/Geometry/Mesh.h"

namespace Razor {

	std::vector<FileBrowser::File> FileBrowser::filesInScope = {};

	FileBrowser::FileBrowser()
	{
		selection = 1;
		currentPath = fs::current_path();
		filepath = currentPath.string();
		oldVisibility = false;
		currentPathIsDir = true;
	}

	FileBrowser::~FileBrowser()
	{
	}

	void FileBrowser::itemClicked(int id)
	{
		std::string path = filesInScope[id].path.string();
		bool isDir = (fs::is_directory(path));
		std::cout << "item clicked: " << path << ", isDir: " <<  isDir << std::endl;

		if (!isDir) {
			Mesh mesh;
			tasksManager->add({
				&mesh, 
				&AssetsManager::import,
				&AssetsManager::finished, 
				Variant(path),
				"Import task"
			});
		}
	}

	void FileBrowser::render()
	{
		bool result = false;
		ImGui::PushID("Columns");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::Columns(2, "twoColumns", true);
		ImGui::PopStyleVar();

		if (oldVisibility != isVisible)
		{
			oldVisibility = isVisible;

			if (isVisible) {
				currentPath = fs::current_path();
				currentPathIsDir = true;

				ImGui::PushItemWidth(-1);
				ImGui::TextWrapped(currentPath.string().data());
				ImGui::PopItemWidth();

				getFiles(currentPath, filesInScope);
			}
		}

		ImGui::PushItemWidth(-1);

		if (ImGui::ListBox("##", &selection, getFromVector, &filesInScope, (int)filesInScope.size(), 10, &FileBrowser::itemClicked)) {

			currentPath = filesInScope[selection].path;
			currentPathIsDir = fs::is_directory(currentPath);

			if (currentPathIsDir)
				getFiles(currentPath, filesInScope);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Text(currentPath.string().data());

		ImGui::PopID();
	
	}

	void FileBrowser::getFiles(const fs::path& path, std::vector<File>& files)
	{
		files.clear();

		if (path.has_parent_path())
			files.push_back({"..", path.parent_path()});

		for (const fs::directory_entry& dirEntry : fs::directory_iterator(path)) {
			const fs::path& dirPath = dirEntry.path();

			if(fs::is_directory(dirPath.string().data()))
				files.push_back({dirPath.filename().string(), dirPath });
		}
	}

	const int FileBrowser::clampToInt(const size_t data)
	{
		static const int max_int = std::numeric_limits<int>::max();
		return static_cast<int>(data > max_int ? max_int : data);
	}

	bool FileBrowser::getFromVector(void* data, int idx, const char** out_text)
	{
		const std::vector<File>* v = reinterpret_cast<std::vector<File>*>(data);
		const int elementCount = clampToInt(v->size());

		if (idx < 0 || idx >= elementCount)
			return false;

		*out_text = v->at(idx).name.data();

		return true;
	}

}

