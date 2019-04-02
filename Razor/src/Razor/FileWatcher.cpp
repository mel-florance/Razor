#include "rzpch.h"
#include "FileWatcher.h"

namespace Razor {

	FileWatcher::FileWatcher()
	{
	}

	FileWatcher::~FileWatcher()
	{
	}

	std::vector<std::string> FileWatcher::tail(const std::string& path, int n)
	{
		std::ifstream file(path);
		std::string line;
		std::vector<std::string> buffer, out;
		int i = 0, j = 0;

		while (std::getline(file, line))
		{
			if (!line.empty()) {
				buffer.push_back(line);
				i++;
			}
		}

		i = 0;
		for (j = buffer.size() - 1; buffer.size() > j && i < n; --j, ++i)
			out.push_back(buffer[j]);

		return out;
	}

}
