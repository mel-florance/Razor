#pragma once

#include "Razor/Core.h"
#include "Razor/FileWatcher.h"

namespace Razor {

	class RAZOR_API AssetsManager
	{
	public:
		AssetsManager();
		~AssetsManager();

		void watch();
		static void setup();

	private:
		FileWatcher* fileWatcher;
	};

}

