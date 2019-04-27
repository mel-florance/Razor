#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct GLFWwindow;

namespace Razor {

	class Window;
	class Editor;
	class Application;

	class Camera
	{
	public:
		Camera(Window* window);
		virtual ~Camera();

		enum class Direction {
			FORWARD,
			BACKWARD, 
			LEFT, 
			RIGHT, 
			UP,
			DOWN
		};

		enum class Mode { ORTHOGRAPHIC, PERSPECTIVE };

		virtual void update(double dt) {}

		inline glm::mat4 getViewMatrix() { return view; }
		inline glm::mat4 getProjectionMatrix() { return projection; }
		inline glm::vec3 getPosition() { return position; }
		inline glm::vec3 getTarget() { return direction; }
		inline Window* getWindow() { return window; }
		inline float getAspectRatio() { return aspect_ratio; }
		inline float getFov() { return fov; }

		virtual void onEvent(Window* window) {}
		virtual void onKeyPressed(Direction direction) {}
		virtual void onKeyDown(int keyCode) {}
		virtual void onKeyReleased(int keyCode) {}
		virtual void onMouseMoved(glm::vec2& offset, bool constrain = true) {}
		virtual void onMouseScrolled(glm::vec2& offset) {}
		virtual void onMouseDown(int button) {}
		virtual void onMouseUp(int button) {}
		virtual void onWindowResized(const glm::vec2& size) {}

		glm::vec3 position;
		glm::vec3 position_delta;
		glm::vec3 direction;

	protected:
		Window* window;

		glm::mat4 projection;
		glm::mat4 view;

		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 world_up;

		Mode mode;
		float fov;
		float aspect_ratio;
		float clip_near;
		float clip_far;

		float yaw;
		float pitch;

		float delta;
		bool first;
		glm::vec2 last_pos;
		bool capture;
		bool isViewportHovered;
	};

}
