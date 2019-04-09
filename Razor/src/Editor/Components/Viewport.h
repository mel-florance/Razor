#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API Viewport : public EditorComponent
	{
	public:
		Viewport(Editor* editor);
		~Viewport();

		void render() override;
	};

}
