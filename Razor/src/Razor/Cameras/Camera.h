#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

struct GLFWwindow;

namespace Razor 
{

	class Window;
	class Viewport;
	class Application;

	class Camera
	{
	public:
		Camera(Window* window);
		virtual ~Camera();

		enum class Direction
		{
			FORWARD,
			BACKWARD, 
			LEFT, 
			RIGHT, 
			UP,
			DOWN
		};

		enum class Mode { ORTHOGRAPHIC, PERSPECTIVE };

		virtual void update(double dt) {}

		inline Viewport* getViewport() { return viewport; }
		inline Window* getWindow() { return window; }

		inline glm::mat4& getViewMatrix() { return view; }
		inline glm::mat4& getProjectionMatrix() { return projection; }

		inline virtual glm::mat4 getRotationMatrix()
		{
			glm::mat4 rot_x = glm::rotate(-glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 rot_y = glm::rotate(-glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

			return rot_x * rot_y;
		}

		inline glm::vec3& getPosition() { return position; }

		inline float& getPositionX() { return position.x; }
		inline float& getPositionY() { return position.y; }
		inline float& getPositionZ() { return position.z; }

		inline glm::vec3& getDirection() { return direction; }
		inline void setDirection(const glm::vec3& dir) { direction = dir; }
		inline glm::vec3& getVelocity() { return velocity; }

		inline glm::vec3& getTarget() { return target; }
		inline void setTarget(const glm::vec3& target) { this->target = target; }

		inline bool isHavingTarget() { return has_target; }
		inline void setHasTarget(bool value) { has_target = value; }

		inline float& getSpeed() { return speed; }
		inline float& getMinSpeed() { return min_speed; }
		inline float& getMaxSpeed() { return max_speed; }

		inline Mode& getMode() { return mode; }
		inline void setMode(Mode mode) { this->mode = mode; }
		inline float& getFov() { return fov; }
		inline float& getClipNear() { return clip_near; }
		inline float& getClipFar() { return clip_far; }
		inline float& getAspectRatio() { return aspect_ratio; }

		virtual void onEvent(Window* window) {}
		virtual void onKeyPressed(Direction direction) {}
		virtual void onKeyDown(int keyCode) {}
		virtual void onKeyReleased(int keyCode) {}
		virtual void onMouseMoved(glm::vec2& offset, bool constrain = true) {}
		virtual void onMouseScrolled(glm::vec2& offset) {}
		virtual void onMouseDown(int button) {}
		virtual void onMouseUp(int button) {}
		virtual void onWindowResized(const glm::vec2& size) {}

	protected:
		Window* window;
		Viewport* viewport;

		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 direction;
		glm::vec3 target;

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
		float roll;

		float delta;
		bool first;
		bool has_target;
		glm::vec2 last_pos;
		bool capture;
		float speed;
		float speed_factor;
		float min_speed;
		float max_speed;
	};

}
