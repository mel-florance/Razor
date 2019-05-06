#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"
#include <glm/glm.hpp>

namespace Razor {

	class GridAxis;

	class RAZOR_API Viewport : public EditorComponent
	{
	public:
		Viewport(Editor* editor);
		~Viewport();

		void render(float delta) override;

		inline glm::vec2& getSize() { return size; }
		inline bool getIsHovered() { return hovered; }
		inline bool getIsFullscreen() { return is_fullscreen; }
		void toggleFullscreen();
		void toggleProperties();

	private:
		glm::vec2 size;
		bool hovered;
		bool show_properties;
		bool grid_opened;
		bool cursor_opened;
		bool view_opened;
		bool show_grid;
		bool show_grid_axis;
		int grid_size;
		float viewport_fov;
		float viewport_clip_start;
		float viewport_clip_end;
		bool is_fullscreen;
		glm::vec3 cursor_position;
		GridAxis* grid_axis;
	};

}
