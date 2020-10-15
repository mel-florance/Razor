#pragma once

#include <glm/glm.hpp>
#include "Razor/Maths/Maths.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

namespace Razor
{

	class Camera;
	class Window;
	
	class Raycast
	{
	public:
		Raycast(Window& window, Camera* camera);
		~Raycast();

		void update();
		glm::vec3 computeRay();
		glm::vec2 getNormalizedCoords(const glm::vec2& position);
		glm::vec4 toEyeCoords(const glm::vec4& clip);
		glm::vec3 toWorldCoords(const glm::vec4& eye);
		
		void test();

		bool intersects(const AABB& box);

		void onMouseDown(int button);
		void onMouseUp(int button);

		inline glm::vec3& getRay() { return ray; }
		glm::vec3 scaleRay(float distance);
		inline void setViewport(const glm::vec2& size) { viewport = size; }

	private:
		glm::vec3 ray;
		glm::vec2 mouse;
		glm::vec2 viewport;
		glm::mat4 projection;
		glm::mat4 view;
		Camera* camera;
		Window& window;

	};

}
