#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class File
	{
	public:
		inline static std::string read(const std::string& path)
		{
			std::ifstream file(path);
			std::string out, line;

			if (file.is_open())
			{
				while (file.good())
				{
					getline(file, line);
					out += line + "\n";
				}

				file.close();
			}

			return out;
		}
	};

}



