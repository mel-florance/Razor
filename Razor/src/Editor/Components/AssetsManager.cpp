#include "rzpch.h"
#include "AssetsManager.h"

namespace Razor {

	AssetsManager::AssetsManager()
	{
		this->fileWatcher = new FileWatcher({ "./", std::chrono::milliseconds(500) });
	}

	void AssetsManager::watch()
	{
		std::thread fileWatcher_thread(&FileWatcher::start, this->fileWatcher, [](std::string path_to_watch, FileWatcher::Status status)
		{
			if (!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileWatcher::Status::erased) {
				return;
			}

			switch (status) {
			case FileWatcher::Status::created:
				RZ_CORE_INFO("File created: {0}", path_to_watch);
				break;
			case FileWatcher::Status::modified:
				RZ_CORE_INFO("File modified: {0}", path_to_watch);
				break;
			case FileWatcher::Status::erased:
				RZ_CORE_WARN("File erased: {0}", path_to_watch);
				break;
			default:
				RZ_CORE_ERROR("Error! Unknown file status.");
			}
		});

		fileWatcher_thread.detach();
	}

	void AssetsManager::setup()
	{

	}

	AssetsManager::~AssetsManager()
	{
	}

}