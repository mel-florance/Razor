#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class Editor;

	class RAZOR_API EditorComponent
	{
	public:
		EditorComponent(Editor* editor = nullptr);
		~EditorComponent();

		virtual void render() {};

		Editor* getEditor();

	protected:
		Editor* editor;
		bool isDirty;
	};

}
