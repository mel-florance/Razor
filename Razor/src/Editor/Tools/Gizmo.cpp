#include "rzpch.h"
#include "Gizmo.h"

#include "Razor/Input/Input.h"
#include "Razor/Input/KeyCodes.h"
#include "Razor/Scene/Node.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Cameras/FPSCamera.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_major_storage.hpp>

namespace Razor
{

	Gizmo::Gizmo(Editor* editor) :
		EditorTool(editor),
		viewport_position(glm::vec2(0.0f)),
		viewport_size(glm::vec2(1280.0f, 720.0f)),
		current_operation(ImGuizmo::OPERATION::TRANSLATE),
		current_mode(ImGuizmo::MODE::WORLD),
		use_snap(false),
		bound_sizing(false),
		bound_sizing_snap(false),
		axis_y_flipped(false),
		snap(glm::vec3(0.1f)),
		bounds(glm::mat2x3(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f)),
		bounds_snap(glm::vec3(0.1f))
	{
	}

	Gizmo::~Gizmo()
	{
	}

	void Gizmo::update()
	{
		if (node_ptr != nullptr && camera != nullptr)
		{
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(
				viewport_position.x,
				viewport_position.y,
				viewport_size.x,
				viewport_size.y
			);

			use_snap = Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL);

			
			glm::mat4 view = camera->getViewMatrix();

			glm::mat4 proj = glm::perspectiveFov(
				camera->getFov(),
				viewport_size.x,
				viewport_size.y,
				camera->getClipNear(),
				camera->getClipFar()
			);

			TPSCamera* cam = static_cast<TPSCamera*>(camera);

			view[0][1] = -view[0][1];
			view[1][1] = -view[1][1];
			view[2][1] = -view[2][1];

			glm::mat4 object = node_ptr->transform.getMatrix();

			ImGuizmo::Manipulate(
				glm::value_ptr(view),
				glm::value_ptr(proj),
				current_operation,
				current_mode,
				glm::value_ptr(object),
				glm::value_ptr(delta_matrix),
				use_snap ? &snap[0] : NULL,
				bound_sizing ? &bounds[0][0] : NULL,
				bound_sizing_snap ? &bounds_snap[0] : NULL
			);

			float matrixTranslation[3], matrixRotation[3], matrixScale[3];

			ImGuizmo::DecomposeMatrixToComponents(
				glm::value_ptr(object),
				matrixTranslation, 
				matrixRotation,
				matrixScale
			);

			node_ptr->transform.setPosition(glm::vec3(
				matrixTranslation[0], 
				matrixTranslation[1], 
				matrixTranslation[2]
			));

			node_ptr->transform.setRotation(glm::vec3(
				glm::radians(matrixRotation[0]),
				glm::radians(matrixRotation[1]),
				glm::radians(matrixRotation[2])
			));

			node_ptr->transform.setScale(glm::vec3(
				matrixScale[0],
				matrixScale[1],
				matrixScale[2]
			));
		}
	}

}
