#pragma once

#include "rzpch.h"
#include "Razor/Core/Core.h"

namespace Razor {

	class RAZOR_API FileWatcher
	{
	public:
		FileWatcher();
		~FileWatcher();

		std::vector<std::string> tail(const std::string& path, int n);
	};

}
