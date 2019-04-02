#include "rzpch.h"
#include "imgui.h"
#include "AssetsManager.h"
#include "Logger.h"
#include "Editor/Editor.h"

namespace Razor {

	AssetsManager::AssetsManager(Editor* editor) : EditorComponent(editor)
	{
		this->directoryWatcher = std::make_shared<DirectoryWatcher>("./", std::chrono::milliseconds(500));
		this->fileWatcher = std::make_shared<FileWatcher>();
		watch();
	}

	void AssetsManager::watch()
	{
		std::thread directoryWatcher_thread(&DirectoryWatcher::start, this->directoryWatcher, [=](std::string path_to_watch, DirectoryWatcher::Status status)
		{
			if (!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != DirectoryWatcher::Status::erased) {
				return;
			}

			switch (status) {
			case DirectoryWatcher::Status::created:
				RZ_CORE_INFO("File created: {0}", path_to_watch);
				break;
			case DirectoryWatcher::Status::modified:
				if (path_to_watch == "./Razor.log") {
					std::vector<std::string> lines = fileWatcher->tail("./Razor.log", 1);
					Logger* logger = (Logger*)this->getEditor()->getComponents()["Logger"];

					for (auto line : lines)
						logger->addLog(("\n" + line).c_str());
				}
				RZ_CORE_INFO("File modified: {0}", path_to_watch);
				break;
			case DirectoryWatcher::Status::erased:
				RZ_CORE_WARN("File erased: {0}", path_to_watch);
				break;
			default:
				RZ_CORE_ERROR("Error! Unknown file status.");
			}
		});

		directoryWatcher_thread.detach();
	}

	void AssetsManager::render()
	{
		ImGui::Begin("Assets Manager");
		ImGui::End();
	}

	AssetsManager::~AssetsManager()
	{
	}

}