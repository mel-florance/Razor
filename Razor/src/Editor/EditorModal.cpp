#include "rzpch.h"
#include "EditorModal.h"
#include "Editor/Editor.h"

namespace Razor
{

	EditorModal::EditorModal(Editor* editor) :
		editor(editor),
		active(false)
	{
	}

	EditorModal::~EditorModal()
	{
	}

}
