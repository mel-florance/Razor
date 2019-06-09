#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class Selection;
	class Texture;

	class RAZOR_API Tools : public EditorComponent
	{
	public:
		Tools(Editor* editor);
		~Tools();

		void render(float delta) override;
		void togglePanel();
		inline void showCubeProps() { show_cube_props = true; }
		inline void showPlaneProps() { show_plane_props = true; }
		inline void showUVSphereProps() { show_uvsphere_props = true; }
		inline void showDirectionalProps() { show_directional_props = true; }
		inline void showPoint() { show_point_props = true; }
		inline void showSpotProps() { show_spot_props = true; }
 
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

		struct DirectionalParameters
		{
			glm::vec3 direction;
		};

		struct PointParameters
		{
			glm::vec3 position;
		};

		struct SpotParameters
		{
			glm::vec3 position;
			glm::vec3 direction;
		};

		struct LandscapeParameters
		{
			
		};

	private:
		glm::vec2 size;
		bool panel_visible;

		bool primitives_opened;
		bool lights_opened;
		bool cameras_opened;
		bool physics_opened;
		bool landscape_opened;

		bool show_cube_props;
		bool show_uvsphere_props;
		bool show_plane_props;
		bool show_directional_props;
		bool show_point_props;
		bool show_spot_props;
		bool show_landscape_props;

		CubeParameters cube_parameters;
		UVSphereParameters UVsphere_parameters;
		PlaneParameters plane_parameters;

		DirectionalParameters directional_parameters;
		PointParameters point_parameters;
		SpotParameters spot_parameters;

		Selection* selection;

		Texture* checkerMap;
	};

}
