#pragma once

#include "Razor/Core.h"
#include "Editor/EditorComponent.h"
#include "Razor/DirectoryWatcher.h"
#include "Razor/FileWatcher.h"
#include "Razor/TasksManager.h"
#include "FileBrowser.h"

namespace Razor {

	class RAZOR_API AssetsManager : public EditorComponent
	{
	public:
		AssetsManager(Editor* editor);
		~AssetsManager();

		void watch();
		void render() override;
		static void import(void* result, TaskFinished tf, Variant opts);
		static void finished(void* result);

		enum Type { None, Model, Image, Audio, Video };
		typedef std::map<Type, std::vector<const char*>> ExtsMap;

		inline static Type getExt(const std::string& str)
		{
			ExtsMap::iterator it;

			for (it = AssetsManager::exts.begin(); it != AssetsManager::exts.end(); ++it)
			{
				auto item = std::find(it->second.begin(), it->second.end(), str);

				if (item != it->second.end())
					return it->first;
			}

			return Type::None;
		}

	private:
		static std::map<Type, std::vector<const char*>> exts;
		std::shared_ptr<DirectoryWatcher> directoryWatcher;
		std::shared_ptr<FileWatcher> fileWatcher;
		std::shared_ptr<FileBrowser> fileBrowser;
	};

}

