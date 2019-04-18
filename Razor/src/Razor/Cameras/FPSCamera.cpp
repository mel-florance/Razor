#include "rzpch.h"
#include "FPSCamera.h"
#include <GLFW/glfw3.h>
#include "Razor/Core/Window.h"
#include "Editor/Components/Viewport.h"
#include "Razor/Application/Application.h"
#include "Editor/Editor.h"

namespace Razor {

	FPSCamera::FPSCamera(Window* window) :
		Camera(window),
		sensitivity(0.03f),
		speed(0.001f),
		min_speed(0.0f),
		max_speed(1.0f),
		view_friction(0.0f),
		move_friction(0.85f),
		mouse_offset(glm::vec2()),
		constrain_pitch(true)
	{
		projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, clip_near, clip_far);
		updateVectors();
		view = glm::lookAt(position, position + target, up);
	}

	FPSCamera::~FPSCamera()
	{
	}

	void FPSCamera::updateVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		target = glm::normalize(front);

		right = glm::normalize(glm::cross(target, world_up));
		up = glm::normalize(glm::cross(right, target));
	}

	void FPSCamera::update(double dt)
	{
		delta = (float)dt;
	}

	void FPSCamera::onEvent(Window * window)
	{
		GLFWwindow* native = (GLFWwindow*)window->GetNativeWindow();
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		if (vp != nullptr) 
		{
			isViewportHovered = vp->getIsHovered();
			aspect_ratio = (float)vp->getSize().x / (float)vp->getSize().y;
		}

		if (capture)
		{
			if (glfwGetKey(native, GLFW_KEY_W) == GLFW_PRESS)
				onKeyPressed(Direction::FORWARD);
			if (glfwGetKey(native, GLFW_KEY_S) == GLFW_PRESS)
				onKeyPressed(Direction::BACKWARD);
			if (glfwGetKey(native, GLFW_KEY_A) == GLFW_PRESS)
				onKeyPressed(Direction::LEFT);
			if (glfwGetKey(native, GLFW_KEY_D) == GLFW_PRESS)
				onKeyPressed(Direction::RIGHT);
			if (glfwGetKey(native, GLFW_KEY_Q) == GLFW_PRESS)
				onKeyPressed(Direction::UP);
			if (glfwGetKey(native, GLFW_KEY_E) == GLFW_PRESS)
				onKeyPressed(Direction::DOWN);
		}

		
		switch (mode) {
			case Mode::ORTHOGRAPHIC:
				projection = glm::ortho(-1.5f * aspect_ratio, 1.5f * aspect_ratio, -1.5f, 1.5f, -10.0f, 10.0f);
				updateVectors();
				break;

			case Mode::PERSPECTIVE:
				projection = glm::perspective(glm::radians(fov), aspect_ratio, clip_near, clip_far);
				updateVectors();
				break;
		}

		position_delta *= move_friction;
		position += position_delta;

		view = glm::lookAt(position, position + target, up);
	}

	void FPSCamera::onKeyPressed(Direction dir)
	{
		if (dir == Direction::FORWARD)
			position_delta += target * speed * delta;
		if (dir == Direction::BACKWARD)
			position_delta -= target * speed * delta;
		if (dir == Direction::LEFT)
			position_delta -= right * speed * delta;
		if (dir == Direction::RIGHT)
			position_delta += right * speed * delta;
		if (dir == Direction::UP)
			position_delta += up * speed * delta;
		if (dir == Direction::DOWN)
			position_delta -= up * speed * delta;
	}

	void FPSCamera::onMouseMoved(glm::vec2& pos, bool constrain)
	{
		if (first)
		{
			last_pos = pos;
			first = false;
		}

		mouse_offset = pos - last_pos;
		last_pos = pos;

		if (capture)
		{
			yaw += mouse_offset.x * delta * sensitivity;
			pitch += mouse_offset.y * delta * sensitivity;

			if (constrain)
			{
				if (pitch > 89.0f)  pitch = 89.0f;
				if (pitch < -89.0f) pitch = -89.0f;
			}

			updateVectors();
		}
	}

	void FPSCamera::onMouseScrolled(glm::vec2 & offset)
	{
		speed += offset.y * 0.00001f * delta;

		if (speed < min_speed)
			speed = min_speed;

		if (speed > max_speed)
			speed = max_speed;
	}

	void FPSCamera::onMouseDown(int button)
	{
		if (button == 1)
		{
			capture = true;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	void FPSCamera::onMouseUp(int button)
	{
		if (button == 1)
		{
			capture = false;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void FPSCamera::onWindowResized(const glm::vec2 & size)
	{
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		if (vp != nullptr) {
			aspect_ratio = (float)vp->getSize().x / (float)vp->getSize().y;
		}

	}

}
