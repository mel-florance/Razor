#include "rzpch.h"
#include "ToolsManager.h"

#include "Editor/Tools/Selection.h"
#include "Editor/Tools/Gizmo.h"
#include "Editor/Tools/GridAxis.h"
#include "Editor/Tools/Cursor.h"

namespace Razor
{

	ToolsManager::ToolsManager(Editor* editor) : EditorManager(editor)
	{
		tools["selection"] = new Selection(editor);
		tools["gizmo"]     = new Gizmo(editor);
		tools["grid_axis"] = new GridAxis(editor);
		tools["cursor"]    = new Cursor(editor);
	}

	ToolsManager::~ToolsManager()
	{

	}

}
