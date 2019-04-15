#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API ProjectsManager : public EditorComponent
	{
	public:
		ProjectsManager(Editor* editor);
		~ProjectsManager();

		typedef std::map<std::string, std::map<std::string, std::string>> ProjectsList;

		struct Project {
			std::string name;
			std::string path;
			unsigned int last_modified;
			unsigned int order;
		};

		void loadProjects();
		inline std::vector<Project>& getProjects() { return projects; }

	private:
		std::vector<Project> projects;
	};

}
