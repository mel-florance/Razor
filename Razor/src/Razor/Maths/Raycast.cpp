#include "rzpch.h"
#include "Raycast.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Razor
{
	Raycast::Raycast(Window* window, Camera* camera, const glm::mat4& projection) : 
		window(window),
		camera(camera),
		projection(projection),
		view(camera->getViewMatrix())
	{
	}

	Raycast::~Raycast()
	{
	}

	void Raycast::update()
	{
		view = camera->getViewMatrix();
		ray = computeRay();
	}

	glm::vec2 Raycast::getNormalizedCoords(const glm::vec2& position)
	{
		return glm::vec2(
			(2.0f * position.x) / window->GetWidth() - 1.0f,
			(2.0f * position.y) / window->GetHeight() - 1.0f
		);
	}

	glm::vec4 Raycast::toEyeCoords(const glm::vec4& clip)
	{
		glm::vec4 coords = glm::inverse(projection) * clip;
		return glm::vec4(coords.x, coords.y, -1.0f, 1.0f);
	}

	glm::vec3 Raycast::toWorldCoords(const glm::vec4& eye)
	{
		glm::vec4 coords = glm::inverse(view) * eye;
		return glm::normalize(glm::vec3(coords.x, coords.y, coords.z));
	}

	bool Raycast::intersects(const BoundingBox& box)
	{
		glm::vec3 r = computeRay();
		glm::vec3 p = camera->getPosition();

		float t1 = (box.min_x - p.x) * r.x;
		float t2 = (box.max_x - p.x) * r.x;

		float t3 = (box.min_y - p.y) * r.y;
		float t4 = (box.max_y - p.y) * r.y;

		float t5 = (box.min_z - p.z) * r.z;
		float t6 = (box.max_z - p.z) * r.z;

		float min = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float max = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (max < 0)
			return false;

		if (min > max)
			return false;

		return true;
	}

	glm::vec3 Raycast::computeRay()
	{
		glm::vec2 coords = getNormalizedCoords(mouse);
		glm::vec4 clip = glm::vec4(coords.x, coords.y, -1.0f, 1.0f); 
		glm::vec4 eye = toEyeCoords(clip);
		glm::vec3 world = toWorldCoords(eye);
		
		return world;
	}
	
	void Raycast::onMouseDown(int button)
	{
	}

	void Raycast::onMouseUp(int button)
	{
		double x, y;
		glfwGetCursorPos((GLFWwindow*)window->GetNativeWindow(), &x, &y);
		mouse = glm::vec2(x, y); // Maybe return -y ? 
	}

}
