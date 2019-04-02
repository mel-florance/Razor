#pragma once

#include "Razor/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API Tools : public EditorComponent
	{
	public:
		Tools(Editor* editor);
		~Tools();

		void render() override;
	};

}
