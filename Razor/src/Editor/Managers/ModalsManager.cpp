#include "rzpch.h"
#include "ModalsManager.h"

#include "Editor/Modals/Projects/CreateProject.h"
#include "Editor/Modals/Assets/ImportAsset.h"
#include "Editor/Modals/Tools/AddShape.h"

namespace Razor
{

	ModalsManager::ModalsManager(Editor* editor) : EditorManager(editor)
	{
		modals["create_project"] = new CreateProject(editor);
		modals["import_asset"]   = new ImportAsset(editor);
		modals["add_shape"]      = new AddShape(editor);
	}

	ModalsManager::~ModalsManager()
	{

	}

}
