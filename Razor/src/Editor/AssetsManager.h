#pragma once

#include "Razor/Core.h"
#include "Razor/FileWatcher.h"

namespace Razor {

	class RZ_API AssetsManager
	{
	public:
		AssetsManager();
		~AssetsManager();

	private:
		FileWatcher* fileWatcher;
	};

}

