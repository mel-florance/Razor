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

		inline ImVec2& getSize() { return size; }

	private:
		ImVec2 size;
	};

}
