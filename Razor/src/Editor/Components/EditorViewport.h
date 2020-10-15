#pragma once

#include "Razor/Core/Core.h"


#include "Editor/EditorComponent.h"
#include <glm/glm.hpp>

namespace Razor {

	class GridAxis;
	class Gizmo;
	class Selection;
	class Viewport;
	class Renderer;
	class GBuffer;

	class EditorViewport : public EditorComponent
	{
	public:
		EditorViewport(Editor* editor);
		~EditorViewport();

		void render(float delta) override;
		void onEvent(Event& event) override;

		bool isHovered();
		inline glm::vec2& getSize() { return size; }
		inline bool isFullscreen() { return is_fullscreen; }
		void toggleFullscreen();
		void toggleProperties();
		void showContextMenu();

	private:
		glm::vec2 size;
		bool show_fps;
		bool show_properties;
		bool grid_opened;
		bool cursor_opened;
		bool view_opened;
		bool show_grid;
		bool show_grid_axis;
		bool show_context_menu;
		glm::vec2 context_menu_position;
		int grid_size;
		float viewport_fov;
		float viewport_clip_start;
		float viewport_clip_end;
		bool is_fullscreen;
		glm::vec3 cursor_position;
		GridAxis* grid_axis;
		Gizmo* gizmo;
		Selection* selection;
		Viewport* viewport;
		Renderer* renderer;
		GBuffer* g_buffer;

		unsigned int current_pass;
	};

}
