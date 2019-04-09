#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;


namespace Razor {

	class Window;

	class Camera
	{
	public:
		Camera(
			Window* window,
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), 
			const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = -90.0f, 
			float pitch = 0.0f
		);
		~Camera();

		enum class Direction {
			FORWARD,
			BACKWARD, 
			LEFT, 
			RIGHT, 
			UP,
			DOWN
		};

		void update();
		inline glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }
		inline glm::vec3& getPosition() { return position; }
		inline glm::vec3& getFront() { return front; }
		inline Window* getWindow() { return window; }

		void onEvent(GLFWwindow* window);

		void onKeyPressed(Direction direction);
		void onMouseMoved(glm::vec2& offset, bool constrain = true);
		void onMouseScrolled(glm::vec2& offset);
		void onMouseDown(int button);
		void onMouseUp(int button);

		inline float getZoom() { return zoom; }
		inline void setDelta(float delta) { this->delta = delta; }

	private:
		Window* window;

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 world_up;

		float yaw;
		float pitch;

		float speed;
		float sensitivity;
		float zoom;
		float delta;

		bool capture;
		bool first;
		glm::vec2 last_pos;
	};

}
