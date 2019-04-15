#include "rzpch.h"
#include "Config.h"
#include "Razor/Core/Utils.h"

namespace Razor {

	Config::Config(const std::string& filename)
	{
		parse(filename);
	}

	Config::~Config()
	{
	}

	Config::Section* Config::getSection(const std::string& name)
	{
		const auto it = std::find_if(sections.begin(), sections.end(),
			[name](const Section& section) {
			return section.name.compare(name) == 0;
		});

		return it != sections.end() ? &*it : NULL;

		return nullptr;
	}

	std::string Config::getValue(const std::string& section_name, const std::string& key_name)
	{
		Section* section = getSection(section_name);

		if (section != NULL)
		{
			const auto it = section->data.find(key_name);

			if (it != section->data.end())
				return it->second;
		}

		return std::string();
	}

	void Config::parse(const std::string& filename)
	{
		Section current;
		std::ifstream file;
		file.open(filename);

		if(!file)
			RZ_ERROR("Can't open config file: {0}", filename)
	
		for (std::string line; std::getline(file, line);)
		{
			if (!line.empty() && (line[0] == ';' || line[0] == '#')) {

			}
			else if (line[0] == '[')
			{
				size_t end = line.find_first_of(']');

				if (end != std::string::npos)
				{
					if (!current.name.empty())
					{
						sections.push_back(current);
						current.name.clear();
						current.data.clear();
					}

					current.name = line.substr(1, end - 1);
				}
				else {

				}
			}
			else if (!line.empty())
			{
				size_t end = line.find_first_of("=:");

				if (end != std::string::npos)
				{
					std::string name = line.substr(0, end);
					std::string value = line.substr(end + 1);
					Utils::ltrim(Utils::rtrim(name));
					Utils::ltrim(Utils::rtrim(value));

					current.data[name] = value;
				}
			}
			else {

			}
		}

		if (!current.name.empty())
		{
			sections.push_back(current);
			current.name = "";
			current.data.clear();
		}
	}

}
