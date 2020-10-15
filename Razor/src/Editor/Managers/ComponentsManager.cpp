#include "rzpch.h"
#include "ComponentsManager.h"

#include "Editor/Components/AssetsManager.h"
#include "Editor/Components/MainMenu.h"
#include "Editor/Components/Console.h"
#include "Editor/Components/PropertiesEditor.h"
#include "Editor/Components/Tools.h"
#include "Editor/Components/EditorViewport.h"
#include "Editor/Components/EditorView.h"
#include "Editor/Components/Outliner.h"
#include "Editor/Components/Logger.h"
#include "Editor/Components/ProjectsManager.h"
#include "Editor/Components/ImageEditor.h"
#include "Editor/Components/TextEditor.h"
#include "Editor/Components/NotificationsManager.h"
#include "Editor/Components/Sequencer/Sequencer.h"
#include "Editor/Components/WorldEditor.h"
#include "Editor/Components/MaterialEditor/MaterialEditor.h"

namespace Razor
{

	ComponentsManager::ComponentsManager(Editor* editor) : EditorManager(editor)
	{
		components["ProjectsManager"]      = new ProjectsManager(editor);
		components["AssetsManager"]        = new AssetsManager(editor);
		components["Logger"]               = new Logger(editor);
		components["Console"]              = new Console(editor);
		components["MainMenu"]             = new MainMenu(editor);
		components["Outliner"]             = new Outliner(editor);
		components["PropertiesEditor"]     = new PropertiesEditor(editor);
		components["Tools"]                = new Tools(editor);
		components["NotificationsManager"] = new NotificationsManager(editor);
		components["Sequencer"]            = new Sequencer(editor);
		components["WorldEditor"]          = new WorldEditor(editor);
		components["ImageEditor"]          = new ImageEditor(editor);
		components["MaterialEditor"]       = new MaterialEditor(editor);
		components["TextEditor"]           = new TextEditor(editor);
		components["Viewport"]             = new EditorViewport(editor);
	}

	ComponentsManager::~ComponentsManager()
	{

	}

	void ComponentsManager::onEvent(Razor::Event& event)
	{
		ComponentsMap::iterator it_c;
	
		for (it_c = components.begin(); it_c != components.end(); ++it_c)
			if (it_c->second->isActive())
				it_c->second->onEvent(event);
	}

}
