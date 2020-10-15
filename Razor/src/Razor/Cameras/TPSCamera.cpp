#include "rzpch.h"
#include "TPSCamera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Razor/Core/Window.h"
#include "Razor/Core/Transform.h"
#include "Razor/Core/Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Maths/Maths.h"

namespace Razor 
{

	TPSCamera::TPSCamera(Window* window, TPSCamera* cam) :
		Camera(window),
		target(new Transform()),
		sensitivity(cam->getSensitivity()),
		pan_sensitivity(cam->getPanSensitivity()),
		pitch(cam->getPitch()),
		yaw(cam->getYaw()),
		roll(cam->getRoll()),
		distance(cam->getDistance()),
		distance_min(cam->getDistanceMin()),
		distance_max(cam->getDistanceMax()),
		pitch_min(cam->getPitchMin()),
		pitch_max(cam->getPitchMax()),
		pitch_offset(cam->getPitchOffset()),
		pitch_factor(cam->getPitchFactor()),
		zoom_factor(cam->getZoomFactor()),
		angle(cam->getAngle()),
		alpha(cam->getAlpha()),
		y_offset(cam->getYOffset()),
		delta_offset(cam->getDeltaOffset())
	{
	}

	TPSCamera::TPSCamera(Window* window) :
		Camera(window),
		target(new Transform()),
		sensitivity(0.3f),
		pan_sensitivity(0.3f),
		pitch(-45.0f),
		yaw(0.0f),
		roll(0.0f),
		distance(10.0f),
		distance_min(0.1f),
		distance_max(1000.0f),
		pitch_min(-90.0f),
		pitch_max(90.0f),
		pitch_offset(glm::vec2(0.0f)),
		pitch_factor(0.3f),
		zoom_factor(3.0f),
		angle(45.0f),
		alpha(0.0f),
		y_offset(0.0f),
		delta_offset(glm::vec3(0.0f))
	{

	}

	TPSCamera::~TPSCamera()
	{
		delete target;
	}

	void TPSCamera::update(double dt)
	{
		//alpha += 0.5f * dt;
		//y_offset = sin(alpha - dt) - 20.0f;
		//roll += 0.5f * dt;
		angle += 8.0f * dt;

		if (target != nullptr)
		{
			float vertical = distance * std::sin(glm::radians(pitch + pitch_offset.y));
			float horizontal = distance * std::cos(glm::radians(pitch + pitch_offset.x));

			float theta = angle;
			float offset_x = horizontal * std::sin(glm::radians(theta));
			float offset_z = horizontal * std::cos(glm::radians(theta));

			position.x = target->getPosition().x - offset_x;
			position.z = target->getPosition().z - offset_z;
			position.y = target->getPosition().y + vertical + y_offset;

			yaw = 180.0f - (angle + y_offset);
			yaw = (float)fmod(yaw, 360.0f);
		}

		direction = glm::normalize(target->getPosition() - position);
		right = glm::normalize(glm::cross(direction, world_up));
		up = glm::normalize(glm::cross(right, direction));

		view = glm::lookAt(position, target->getPosition(), up);

		setProjection();
	}

	void TPSCamera::setProjection()
	{
		if(viewport != nullptr)
			aspect_ratio = viewport->getSize().x / viewport->getSize().y;

		switch (mode) 
		{
			case Mode::ORTHOGRAPHIC:
				projection = glm::ortho(
					(1.0f + distance) * aspect_ratio,
					(-1.0f - distance) * aspect_ratio,
					1.0f + distance,
					-1.0f - distance,
					-1000.0f, 
					1000.0f
				);
				break;
			case Mode::PERSPECTIVE:
				projection = glm::perspective(glm::radians(fov), aspect_ratio, clip_near, clip_far);
				break;
		}
	}

	void TPSCamera::onMouseMoved(glm::vec2& pos, bool constrain)
	{
		if (first)
		{
			last_pos = pos;
			first = false;
		}

		mouse_position = pos - last_pos;
		last_pos = pos;

		if (capture)
		{
			if (glfwGetKey((GLFWwindow*)window->GetNativeWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				direction = glm::normalize(target->getPosition() - position);

				right = glm::normalize(glm::cross(direction, world_up));
				up = glm::normalize(glm::cross(right, direction));

				if (pitch == 90.0f && yaw == 90.0f)
					mouse_position *= -1.0f;

				if (mouse_position.x > 0)
					velocity -= right * delta * pan_sensitivity * distance;
				if (mouse_position.x < 0)
					velocity += right * delta * pan_sensitivity * distance;
				if (mouse_position.y > 0)
					velocity -= up * delta * pan_sensitivity * distance;
				if (mouse_position.y < 0)
					velocity += up * delta * pan_sensitivity * distance;

				glm::vec3 pos = target->getPosition();
				target->setPosition(pos + velocity);
				velocity = glm::vec3(0.0f);
			}
			else
			{
				float diff = mouse_position.y * sensitivity;
				pitch -= diff;

				if (pitch < pitch_min)
					pitch = pitch_min;
				else if (pitch > pitch_max)
					pitch = pitch_max;

				angle -= mouse_position.x * sensitivity;
			}
		}
	}

	void TPSCamera::onMouseScrolled(glm::vec2& offset)
	{
		if (viewport->isHovered())
		{
			distance -= (offset.y / 10.0f) * zoom_factor;

			if (distance < distance_min)
				distance = distance_min;
			else if (distance > distance_max)
				distance = distance_max;
		}
	}

	void TPSCamera::onMouseDown(int button)
	{
		if (button == 2 && viewport->isHovered())
		{
			capture = true;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	void TPSCamera::onMouseUp(int button)
	{
		if (button == 2)
		{
			capture = false;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void TPSCamera::onKeyDown(int keyCode)
	{
		if (keyCode == GLFW_KEY_KP_5 && viewport->isHovered()) 
		{
			mode = mode == Camera::Mode::ORTHOGRAPHIC ? Camera::Mode::PERSPECTIVE : Camera::Mode::ORTHOGRAPHIC;
			//ImGuizmo::SetOrthographic(mode == Camera::Mode::ORTHOGRAPHIC);
		}
	}

	void TPSCamera::onKeyReleased(int keyCode)
	{
	}

	void TPSCamera::onWindowResized(const glm::vec2& size)
	{
		if (viewport != nullptr) {
			aspect_ratio = (float)viewport->getSize().x / (float)viewport->getSize().y;
		}
	}

	void TPSCamera::setTarget(Transform* transform)
	{
		target->setPosition(glm::vec3(transform->getPosition()));
	}

}
