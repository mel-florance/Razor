#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API Outliner : public EditorComponent
	{
	public:
		Outliner(Editor* editor);
		~Outliner();

		void render() override;
	};

}
