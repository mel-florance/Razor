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

		Editor* getEditor();

		inline bool isActive() { return active; }
		inline bool isDirty() { return dirty; }
		inline void setActive(bool value) { active = value; }
		inline void setDirty(bool value) { dirty = value; }

	private:
		Editor* editor;
		bool dirty;
		bool active;
	};

}
