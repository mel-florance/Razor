#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class ScenesManager;

	class RAZOR_API Outliner : public EditorComponent
	{
	public:
		Outliner(Editor* editor);
		~Outliner();

		void render(float delta) override;

	private:
		ScenesManager* scenesManager;
	};

}
