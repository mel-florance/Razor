#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class RAZOR_API Tools : public EditorComponent
	{
	public:
		Tools(Editor* editor);
		~Tools();

		void render(float delta) override;
		void togglePanel();

		inline const glm::vec2& getSize() const { return size; }

		struct CubeParameters
		{
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);
			float radius = 1.0f;
		};

		struct UVSphereParameters
		{
			glm::ivec2 segments = glm::ivec2(16, 16);
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);
			float radius = 1.0f;
		};

		struct PlaneParameters
		{
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);
			float radius = 1.0f;
		};

	private:
		glm::vec2 size;
		bool panel_visible;
		bool primitives_opened;
		bool lights_opened;
		bool cameras_opened;
		bool show_cube_props;
		bool show_uvsphere_props;
		bool show_plane_props;

		CubeParameters cube_parameters;
		UVSphereParameters UVsphere_parameters;
		PlaneParameters plane_parameters;
	};

}
