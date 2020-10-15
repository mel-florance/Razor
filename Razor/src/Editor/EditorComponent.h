#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class Editor;
	class Event;

	class EditorComponent
	{
	public:
		EditorComponent(Editor* editor);
		~EditorComponent();

		virtual void render(float delta) {};

		Editor* getEditor();

		inline bool& isActive() { return active; }
		inline bool isDirty() { return dirty; }
		inline void setActive(bool value) { active = value; }
		inline void setDirty(bool value) { dirty = value; }

		virtual void onEvent(Event& event) {}

		Editor* editor;
		bool dirty;
		bool active;
	};

}
