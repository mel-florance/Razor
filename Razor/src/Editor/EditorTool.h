#pragma once

namespace Razor
{
	class Editor;

	class EditorTool
	{
	public:
		EditorTool(Editor* editor = nullptr);
		~EditorTool();

		virtual void render(float delta) {};

	private:
		Editor* editor;
	};

}
