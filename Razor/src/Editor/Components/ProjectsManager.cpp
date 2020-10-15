#include "rzpch.h"
#include "ProjectsManager.h"
#include "Razor/Filesystem/Config.h"
#include "Editor/Editor.h"
#include "Editor/Components/AssetsManager.h"

namespace fs = std::filesystem;

namespace Razor 
{

	ProjectsManager::ProjectsManager(Editor* editor) : 
		EditorComponent(editor),
		projects_path("")
	{
		projects_path = "C:\\Users\\overk\\Documents\\RazorProjects";
	}

	ProjectsManager::~ProjectsManager()
	{
	}

	void ProjectsManager::loadRecentProjects()
	{
		/*Config cfg("./config/projects.ini");

		for (auto section : cfg.getSections())
		{
			Project project;
			project.name = section.name;

			for (auto key : section.data)
			{
				if (key.first == "path")
					project.path = key.second;
				if (key.first == "last_modified")
					project.last_modified = static_cast<long int>(std::stol(key.second));
				if (key.first == "order")
					project.order = static_cast<unsigned int>(std::stoul(key.second));
			}

			projects.push_back(project);
		}*/
	}

	void ProjectsManager::createProject(const std::string& name)
	{
		/*AssetsManager* am = editor->getComponent<AssetsManager*>("AssetsManager");
		std::shared_ptr<FileBrowser> fb = am->getFileBrowser();

		std::string home_projects = fb->getHomeProjectsDirectory();
		std::string project_path = home_projects + "/" + name;

		if (!fs::is_directory(project_path) || !fs::exists(project_path))
		{
			Project project;
			project.name = name;
			project.path = project_path;
			project.order = 0;
			project.last_modified = (long int)time(0);

			fs::create_directory(project_path);
			Log::trace("Created project\"%s\" at location: \"%s\"", name.c_str(), project_path.c_str());

			projects.push_back(project);
		}*/
	}

}
