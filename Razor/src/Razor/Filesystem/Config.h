#pragma once

namespace Razor {

	class Config
	{
	public:
		Config(const std::string& filename);
		~Config();

		struct Section {
			std::string name;
			std::unordered_map<std::string, std::string> data;
		};

		Section* getSection(const std::string& name);
		inline const std::list<Section>& getSections() const { return sections; }

		std::string getValue(const std::string& section_name, const std::string& key_name);

	private:
		void parse(const std::string& filename);
		std::list<Section> sections;
	};

}

