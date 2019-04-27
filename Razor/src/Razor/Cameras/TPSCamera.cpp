#include "rzpch.h"
#include "TPSCamera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Razor/Core/Window.h"
#include "Razor/Core/Transform.h"
#include "Editor/Components/Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"
#include "Razor/Maths/Maths.h"

namespace Razor {

	TPSCamera::TPSCamera(Window* window) :
		Camera(window),
		target(new Transform()),
		sensitivity(15.0f),
		pitch(-30.0f),
		yaw(0.0f),
		roll(0.0f),
		distance(5.0f),
		distance_min(0.5f),
		distance_max(1000.0f),
		pitch_min(-90.0f),
		pitch_max(90.0f),
		pitch_offset(0.0f),
		pitch_factor(0.3f),
		zoom_factor(10.0f),
		angle(45.0f),
		y_offset(7.0f)
	{

	}

	TPSCamera::~TPSCamera()
	{
		delete target;
	}

	void TPSCamera::updateVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(front);

		right = glm::normalize(glm::cross(direction, world_up));
		up = glm::normalize(glm::cross(right, direction));
	}

	void TPSCamera::update(double dt)
	{
		if (target != nullptr)
		{
			float vertical = distance * std::sin(glm::radians(pitch + pitch_offset));
			float horizontal = distance * std::cos(glm::radians(pitch + pitch_offset));

			float theta = target->getRotation().y + angle;
			float offset_x = horizontal * std::sin(glm::radians(theta));
			float offset_z = horizontal * std::cos(glm::radians(theta));

			position.x = target->getPosition().x - offset_x;
			position.z = target->getPosition().z - offset_z;
			position.y = target->getPosition().y + vertical;

			yaw = 180.0f - (target->getRotation().y + angle);
			yaw = (float)fmod(yaw, 360.0f);
		}

		glm::mat4 viewMatrix = glm::mat4(1.0f);

		viewMatrix = glm::rotate(viewMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
		view = glm::translate(viewMatrix, glm::vec3(-position[0], -position[1], -position[2]));

		//angle -= delta;

		setProjection();
	}

	void TPSCamera::setProjection()
	{
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		if(vp != nullptr)
			aspect_ratio = vp->getSize().x / vp->getSize().y;

		switch (mode) 
		{
			case Mode::ORTHOGRAPHIC:
				projection = glm::ortho(-1.5f * aspect_ratio, 1.5f * aspect_ratio, -1.5f, 1.5f, -10.0f, 10.0f);
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
				/*glm::vec3 t_pos = target->getPosition();

				t_pos.z -= delta * sensitivity * mouse_position.x;
				t_pos.y -= delta * sensitivity * mouse_position.y;

				target->setPosition(t_pos);*/
			}
			else
			{
				float diff = mouse_position.y * sensitivity * delta;
				pitch -= diff;

				if (pitch < pitch_min)
					pitch = pitch_min;
				else if (pitch > pitch_max)
					pitch = pitch_max;

				angle -= mouse_position.x * sensitivity * delta;
			}
		}
	}

	void TPSCamera::onMouseScrolled(glm::vec2& offset)
	{
		distance -= (offset.y / 10.0f) * zoom_factor;

		if (distance < distance_min)
			distance = distance_min;
		else if (distance > distance_max)
			distance = distance_max;
	}

	void TPSCamera::onMouseDown(int button)
	{
		if (button == 2)
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
		if (keyCode == GLFW_KEY_KP_DECIMAL)
			target->setPosition(glm::vec3(0.0f));

		if (keyCode == GLFW_KEY_KP_5)
			mode = mode == Camera::Mode::ORTHOGRAPHIC ? Camera::Mode::PERSPECTIVE : Camera::Mode::ORTHOGRAPHIC;
	}

	void TPSCamera::onKeyReleased(int keyCode)
	{
	}

}
