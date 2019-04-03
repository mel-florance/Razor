#include "rzpch.h"
#include "EditorComponent.h"
#include "Editor/Editor.h"

namespace Razor {

	EditorComponent::EditorComponent(Editor* editor) : 
		editor(editor),
		isDirty(false)
	{
	}

	EditorComponent::~EditorComponent()
	{
	}

	Editor* EditorComponent::getEditor()
	{
		return editor;
	}

}
