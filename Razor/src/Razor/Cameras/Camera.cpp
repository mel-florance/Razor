#include "rzpch.h"
#include "Camera.h"
#include <GLFW/glfw3.h>
#include "Razor/Core/Window.h"

namespace Razor {

	Camera::Camera(
		Window* window,
		const glm::vec3& position,
		const glm::vec3& up,
		float yaw, 
		float pitch
	) :
		window(window),
		position(position),
		world_up(up),
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		yaw(yaw),
		pitch(pitch),
		sensitivity(150.0f),
		speed(2.5f),
		zoom(90.0f),
		first(true),
		capture(false),
		last_pos(glm::vec2())
	{

		update();
	}

	Camera::~Camera()
	{
	}

	void Camera::update()
	{
		glm::vec3 v;
		v.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
		v.y = sinf(glm::radians(pitch));
		v.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

		front = glm::normalize(v);

		right = glm::normalize(glm::cross(front, world_up));
		up = glm::normalize(glm::cross(right, front));
	}

	void Camera::onEvent(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			onKeyPressed(Direction::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			onKeyPressed(Direction::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			onKeyPressed(Direction::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			onKeyPressed(Direction::RIGHT);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			onKeyPressed(Direction::UP);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			onKeyPressed(Direction::DOWN);
	}

	void Camera::onKeyPressed(Direction direction)
	{
		float velocity = speed * delta;

		if (direction == Direction::FORWARD)
			position += front * velocity;
		if (direction == Direction::BACKWARD)
			position -= front * velocity;
		if (direction == Direction::LEFT)
			position -= right * velocity;
		if (direction == Direction::RIGHT)
			position += right * velocity;
		if (direction == Direction::UP)
			position += up * velocity;
		if (direction == Direction::DOWN)
			position -= up * velocity;
	}

	void Camera::onMouseMoved(glm::vec2& pos, bool constrain)
	{
		if (first)
		{
			last_pos = pos;
			first = false;
		}

		glm::vec2 offset = { pos.x - last_pos.x ,  pos.y - last_pos.y };

		last_pos = pos;

		offset.x *= sensitivity * delta;
		offset.y *= sensitivity * delta;

		if (capture)
		{
			yaw += offset.x;
			pitch += offset.y;

			if (constrain)
			{
				if (pitch > 89.0f)  pitch = 89.0f;
				if (pitch < -89.0f) pitch = -89.0f;
			}

			update();
		}
	}

	void Camera::onMouseScrolled(glm::vec2& offset)
	{
		if (zoom >= 1.0f && zoom <= 45.0) 
			zoom -= offset.y;

		if (zoom <= 1.0f)
			zoom = 1.0f;

		if (zoom >= 45.0f)
			zoom = 45.0f;
	}

	void Camera::onMouseDown(int button)
	{
		if (button == 1)
		{
			capture = true;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	void Camera::onMouseUp(int button)
	{
		if (button == 1)
		{
			capture = false;
			glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

}
