#pragma once

#include "Editor/EditorComponent.h"

namespace Razor
{

	class WorldEditor : public EditorComponent
	{
	public:
		WorldEditor(Editor* editor);
		~WorldEditor();

		void render(float delta) override;
	};

}
