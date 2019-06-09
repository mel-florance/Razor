#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	class Selection;

	class RAZOR_API PropertiesEditor : public EditorComponent
	{
	public:
		PropertiesEditor(Editor* editor);
		~PropertiesEditor();

		void render(float delta) override;

	private:
		bool transform_opened;
		bool static_mesh_opened;
		bool material_opened;
		bool lights_opened;

		char name[32];

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Selection* selection;
		const char* current_material;
		const char* current_light_type;
	};

}
