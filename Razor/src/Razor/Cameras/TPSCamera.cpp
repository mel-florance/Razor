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

namespace Razor {

	TPSCamera::TPSCamera(Window* window) :
		Camera(window),
		target(new Transform()),
		sensitivity(80.0f),
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
		projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, clip_near, clip_far);
	}

	TPSCamera::~TPSCamera()
	{
		delete target;
	}

	void TPSCamera::update(double dt)
	{
		delta = dt;

		move();
		glm::mat4 viewMatrix = glm::mat4(1.0f);

		viewMatrix = glm::rotate(viewMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));

		glm::vec3 negativeCameraPos = glm::vec3(-position[0], -position[1], -position[2]);
		view = glm::translate(viewMatrix, negativeCameraPos);

		GLFWwindow* native = (GLFWwindow*)window->GetNativeWindow();
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		if (vp != nullptr)
			aspect_ratio = (float)vp->getSize().x / (float)vp->getSize().y;

		projection = glm::perspective(glm::radians(fov), aspect_ratio, clip_near, clip_far);
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
			float diff = mouse_position.y * sensitivity * delta;
			pitch -= diff;

			if (pitch < pitch_min)
				pitch = pitch_min;
			else if (pitch > pitch_max)
				pitch = pitch_max;

			angle -= mouse_position.x * sensitivity * delta;
		}
	}

	void TPSCamera::onMouseScrolled(glm::vec2& offset)
	{
		distance -= (offset.y / 10.0f) * zoom_factor;

		if (distance < distance_min)
			distance = distance_min;
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

	void TPSCamera::computePosition(float h_distance, float v_distance)
	{
		if (target != nullptr)
		{
			float theta = target->getRotation().y + angle;
			float offset_x = h_distance * std::sin(glm::radians(theta));
			float offset_z = h_distance * std::cos(glm::radians(theta));

			position.x = target->getPosition().x - offset_x;
			position.z = target->getPosition().z - offset_z;
			position.y = target->getPosition().y + v_distance;
		}
	}

	float TPSCamera::computeHorizontalDistance()
	{
		return distance * std::cos(glm::radians(pitch + pitch_offset));
	}

	float TPSCamera::computeVerticalDistance()
	{
		return distance * std::sin(glm::radians(pitch + pitch_offset));
	}

	void TPSCamera::move()
	{
		float h_dist = computeHorizontalDistance();
		float v_dist = computeVerticalDistance();
		
		computePosition(h_dist, v_dist);

		if (target != nullptr)
		{
			yaw = 180.0f - (target->getRotation().y + angle);
			yaw = (float)fmod(yaw, 360.0f);
		}
	}

}
