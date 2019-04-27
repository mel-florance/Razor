#pragma once

#include "Camera.h"

namespace Razor {

	class Transform;

	class TPSCamera : public Camera
	{
	public:
		TPSCamera(Window* window);
		virtual ~TPSCamera();

		virtual void updateVectors();
		virtual void update(double dt) override;
		virtual void setProjection();

		virtual void onMouseMoved(glm::vec2& pos, bool constrain = true) override;
		virtual void onMouseScrolled(glm::vec2& offset) override;
		virtual void onMouseDown(int button) override;
		virtual void onMouseUp(int button) override;
		virtual void onKeyDown(int keyCode) override;
		virtual void onKeyReleased(int keyCode) override;

	private:
		Transform* target;

		float distance;
		float distance_smooth;
		float distance_min;
		float distance_max;

		float pitch;
		float pitch_min;
		float pitch_max;
		float pitch_offset;
		float pitch_factor;

		float yaw;
		float yaw_min;
		float yaw_max;

		float roll;
		float roll_min;
		float roll_max;
		
		float angle;
		float y_offset;
		float sensitivity;

		float zoom;
		float zoom_factor;
		float zoom_lerp;

		glm::vec2 mouse_scroll;
		glm::vec2 mouse_position;
	};

}
