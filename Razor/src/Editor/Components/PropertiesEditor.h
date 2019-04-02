#pragma once

#include "Razor/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API PropertiesEditor : public EditorComponent
	{
	public:
		PropertiesEditor(Editor* editor);
		~PropertiesEditor();

		void render() override;
	};

}
