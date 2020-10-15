#pragma once

#include "Editor/EditorComponent.h"

namespace Razor
{

	class MaterialEditor : public EditorComponent
	{
	public:
		MaterialEditor(Editor* editor);
		~MaterialEditor();

		void render(float delta) override;
		void onEvent(Event& event) override;
	};

}
