#pragma once

#include "Razor/Core.h"
#include "Editor/EditorComponent.h"
#include "Razor/DirectoryWatcher.h"
#include "Razor/FileWatcher.h"

namespace Razor {

	class RAZOR_API AssetsManager : public EditorComponent
	{
	public:
		AssetsManager(Editor* editor);
		~AssetsManager();

		void watch();
		void render() override;

	private:
		std::shared_ptr<DirectoryWatcher> directoryWatcher;
		std::shared_ptr<FileWatcher> fileWatcher;
	};

}

