#pragma once

namespace Razor
{
	class Editor;

	class EditorManager
	{
	public:
		EditorManager(Editor* editor);
		virtual ~EditorManager();

	protected:
		Editor* editor;
	};

}
