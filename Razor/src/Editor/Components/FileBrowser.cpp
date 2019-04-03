#include "rzpch.h"
#include "imgui.h"
#include "FileBrowser.h"

namespace Razor {

	std::vector<FileBrowser::File> FileBrowser::filesInScope = {};

	FileBrowser::FileBrowser(Editor* editor) : EditorComponent(editor)
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
		std::cout << "item clicked: " << filesInScope[id].path << std::endl;
	}

	void FileBrowser::render()
	{
		bool result = false;

		if (oldVisibility != isVisible) {
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

		if (ImGui::ListBox("##", &selection, getFromVector, &filesInScope, (int)filesInScope.size(), 10, &FileBrowser::itemClicked)) {

			currentPath = filesInScope[selection].path;
			currentPathIsDir = fs::is_directory(currentPath);

			if (currentPathIsDir)
				getFiles(currentPath, filesInScope);

		}


		/*ImGui::Spacing();
		ImGui::SameLine(ImGui::GetWindowWidth() - 200);

		if (currentPathIsDir) {

			static const ImVec4 disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

			ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);

			ImGui::Button("Select");

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}
		else {
			if (ImGui::Button("Select")) {
				ImGui::CloseCurrentPopup();

				filepath = currentPath.string();
				result = true;
			}
		}*/


	}

	void FileBrowser::getFiles(const fs::path & path, std::vector<File>& files)
	{
		files.clear();

		if (path.has_parent_path())
			files.push_back({"..", path.parent_path()});

		for (const fs::directory_entry& dirEntry : fs::directory_iterator(path)) {
			const fs::path& dirPath = dirEntry.path();
			files.push_back({dirPath.filename().string(), dirPath });
		}
	}

	const int FileBrowser::clampToInt(const size_t data)
	{
		static const int max_int = std::numeric_limits<int>::max();
		return static_cast<int>(data > max_int ? max_int : data);
	}

	bool FileBrowser::getFromVector(void * data, int idx, const char ** out_text)
	{
		const std::vector<File>* v = reinterpret_cast<std::vector<File>*>(data);
		const int elementCount = clampToInt(v->size());

		if (idx < 0 || idx >= elementCount)
			return false;

		*out_text = v->at(idx).name.data();

		return true;
	}

}

