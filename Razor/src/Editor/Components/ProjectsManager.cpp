#include "rzpch.h"
#include "ProjectsManager.h"
#include "Razor/Filesystem/Config.h"

namespace Razor {

	ProjectsManager::ProjectsManager(Editor* editor) : 
		EditorComponent(editor),
		projects_path("")
	{
		projects_path = "C:\\Users\\overk\\Documents\\RazorProjects";
	}

	ProjectsManager::~ProjectsManager()
	{
	}

	void ProjectsManager::loadProjects()
	{
		Config cfg("./config/projects.ini");

		for (auto section : cfg.getSections())
		{
			Project project;
			project.name = section.name;

			for (auto key : section.data)
			{
				if (key.first == "path")
					project.path = key.second;
				if (key.first == "last_modified")
					project.last_modified = static_cast<unsigned int>(std::stoul(key.second));
				if (key.first == "order")
					project.order = static_cast<unsigned int>(std::stoul(key.second));
			}

			projects.push_back(project);
		}
	}

}
