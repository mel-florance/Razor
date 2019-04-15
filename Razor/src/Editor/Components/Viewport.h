#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"
#include <glm/glm.hpp>

namespace Razor {

	class RAZOR_API Viewport : public EditorComponent
	{
	public:
		Viewport(Editor* editor);
		~Viewport();

		void render(float delta) override;

		inline glm::vec2& getSize() { return size; }
		inline bool getIsHovered() { return hovered; }

	private:
		glm::vec2 size;
		bool hovered;
	};

}
