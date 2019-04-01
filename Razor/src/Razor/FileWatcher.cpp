#include "rzpch.h"
#include "FileWatcher.h"

namespace fs = std::filesystem;

namespace Razor {

	FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay) :
		path_to_watch { path_to_watch },
		delay { delay }
	{
		for (auto &file : fs::recursive_directory_iterator(path_to_watch)) {
			paths_[file.path().string()] = fs::last_write_time(file);
		}
	}

	void FileWatcher::start(const std::function<void(std::string, Status)>& action)
	{
		FilePaths::iterator it;

		while (running_) {
		
			std::this_thread::sleep_for(delay);

			for (auto it = paths_.rbegin(); it != paths_.rend();) {
				if (!fs::exists(it->first)) {
					action(it->first, Status::erased);
					it = decltype(it){ paths_.erase(std::next(it).base()) };
				}
				else
					++it;
			}

			for (auto &file : fs::recursive_directory_iterator(path_to_watch))
			{
				auto current_file_last_write_time = fs::last_write_time(file);

				if (!contains(file.path().string())) {
					paths_[file.path().string()] = current_file_last_write_time;
					action(file.path().string(), Status::created);
				}
				else {
					if (paths_[file.path().string()] != current_file_last_write_time) {
						paths_[file.path().string()] = current_file_last_write_time;
						action(file.path().string(), Status::modified);
					}
				}
			}
		}
	}

}