#include "rzpch.h"
#include "EditorTool.h"
#include "Editor/Editor.h"

namespace Razor
{
	EditorTool::EditorTool(Editor* editor) : 
		editor(editor),
		dirty(true),
		active(true)
	{
	}

	EditorTool::~EditorTool()
	{
	}

	Editor* EditorTool::getEditor()
	{
		return editor;
	}

}
