#pragma once

#include "Editor/EditorModal.h"

namespace Razor
{
	
	class ProjectsManager;

	class CreateProject : public EditorModal
	{
	public:
		CreateProject(Editor* editor);
		~CreateProject();

		void init() override;
		void render() override;

	private:
		ProjectsManager* projects_manager;
	};

}
