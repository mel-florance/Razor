#include "rzpch.h"
#include "Gizmo.h"

#include "Editor/Editor.h"
#include "Razor/Application/Application.h"
#include "Razor/Core/Engine.h"
#include "Razor/Physics/World.h"
#include "Razor/Input/Input.h"
#include "Razor/Input/KeyCodes.h"
#include "Razor/Scene/Node.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Physics/PhysicsBody.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_major_storage.hpp>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Lighting/Spot.h"

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	m16[0] = temp / temp2;
	m16[1] = 0.0;
	m16[2] = 0.0;
	m16[3] = 0.0;
	m16[4] = 0.0;
	m16[5] = temp / temp3;
	m16[6] = 0.0;
	m16[7] = 0.0;
	m16[8] = (right + left) / temp2;
	m16[9] = (top + bottom) / temp3;
	m16[10] = (-zfar - znear) / temp4;
	m16[11] = -1.0f;
	m16[12] = 0.0;
	m16[13] = 0.0;
	m16[14] = (-temp * zfar) / temp4;
	m16[15] = 0.0;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
	xmax = ymax * aspectRatio;
	Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void Cross(const float* a, const float* b, float* r)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

float Dot(const float* a, const float* b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Normalize(const float* a, float* r)
{
	float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
	r[0] = a[0] * il;
	r[1] = a[1] * il;
	r[2] = a[2] * il;
}

void LookAt(const float* eye, const float* at, const float* up, float* m16)
{
	float X[3], Y[3], Z[3], tmp[3];

	tmp[0] = eye[0] - at[0];
	tmp[1] = eye[1] - at[1];
	tmp[2] = eye[2] - at[2];
	//Z.normalize(eye - at);
	Normalize(tmp, Z);
	Normalize(up, Y);
	//Y.normalize(up);

	Cross(Y, Z, tmp);
	//tmp.cross(Y, Z);
	Normalize(tmp, X);
	//X.normalize(tmp);

	Cross(Z, X, tmp);
	//tmp.cross(Z, X);
	Normalize(tmp, Y);
	//Y.normalize(tmp);

	m16[0] = X[0];
	m16[1] = Y[0];
	m16[2] = Z[0];
	m16[3] = 0.0f;
	m16[4] = X[1];
	m16[5] = Y[1];
	m16[6] = Z[1];
	m16[7] = 0.0f;
	m16[8] = X[2];
	m16[9] = Y[2];
	m16[10] = Z[2];
	m16[11] = 0.0f;
	m16[12] = -Dot(X, eye);
	m16[13] = -Dot(Y, eye);
	m16[14] = -Dot(Z, eye);
	m16[15] = 1.0f;
}


namespace Razor
{

	Gizmo::Gizmo(Editor* editor) :
		EditorTool(editor),
		viewport_position(glm::vec2(0.0f)),
		viewport_size(glm::vec2(1920.0f, 1080.0f)),
		current_operation(ImGuizmo::OPERATION::TRANSLATE),
		current_mode(ImGuizmo::MODE::WORLD),
		use_snap(false),
		bound_sizing(false),
		bound_sizing_snap(false),
		axis_y_flipped(false),
		is_hovered(false),
		is_using(false),
		snap(glm::vec3(0.1f)),
		bounds(glm::mat2x3(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f)),
		bounds_snap(glm::vec3(0.1f))
	{
	}

	Gizmo::~Gizmo()
	{
	}

	void Gizmo::update(float delta)
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
			is_hovered = ImGuizmo::IsOver();
			is_using = ImGuizmo::IsUsing();

			TPSCamera* cam = static_cast<TPSCamera*>(camera);
			ImGuizmo::SetOrthographic(cam->getMode() == Camera::Mode::ORTHOGRAPHIC);

			glm::mat4 target_zero = cam->getViewMatrix();

			glm::mat4 view = target_zero;

			glm::mat4 proj = glm::perspective(
				cam->getFov(),
				viewport_size.x / viewport_size.y,
				cam->getClipNear(),
				cam->getClipFar()
			);

			glm::mat4 object = node_ptr->transform.getMatrix();

			//view[0][1] = -view[0][1];
			//view[1][1] = -view[1][1];
			//view[2][1] = -view[2][1];

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

			if (node_ptr->lights.size() > 0)
			{
				if (node_ptr->lights[0]->getType() == Light::Type::DIRECTIONAL)
				{
					std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(node_ptr->lights[0]);
					directional->setPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
					directional->getLightBound()->update();
				}
				else if (node_ptr->lights[0]->getType() == Light::Type::POINT)
				{
					std::shared_ptr<Point> point = std::dynamic_pointer_cast<Point>(node_ptr->lights[0]);
					point->setPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
					point->getLightBound()->update();
				}
				else if (node_ptr->lights[0]->getType() == Light::Type::SPOT)
				{
					std::shared_ptr<Spot> spot = std::dynamic_pointer_cast<Spot>(node_ptr->lights[0]);
					spot->setPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
					spot->getLightBound()->update();
				}
			}

			if (node_ptr->meshes.size() > 0 && ImGuizmo::IsUsing())
			{
				node_ptr->meshes[0]->updateBoundings(node_ptr->transform);

				if (node_ptr->meshes[0]->getPhysicsBody() != nullptr)
				{
					editor->getEngine()->getPhysicsWorld()->getWorld()->removeRigidBody(node_ptr->meshes[0]->getPhysicsBody()->getBody());
					node_ptr->meshes[0]->getPhysicsBody()->updateTransform();
					editor->getEngine()->getPhysicsWorld()->getWorld()->addRigidBody(node_ptr->meshes[0]->getPhysicsBody()->getBody());
				}
			}
		}
	}

}
