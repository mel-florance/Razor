#pragma once

#include "Razor/Core.h"

namespace Razor {

	class RZ_API FileWatcher
	{
	public:
		FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);

		enum class Status { created, modified, erased };

		void start(const std::function<void(std::string, Status)> &action);

		std::string path_to_watch;
		std::chrono::duration<int, std::milli> delay;

	private:
		typedef std::map<std::string, std::filesystem::file_time_type> FilePaths;
		FilePaths paths_;
		bool running_ = true;

		bool contains(const std::string &key) {
			auto el = paths_.find(key);
			return el != paths_.end();
		}
	};

}
