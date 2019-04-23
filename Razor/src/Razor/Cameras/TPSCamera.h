#pragma once

#include "Camera.h"


namespace Razor {

	class Transform;

	class TPSCamera : public Camera
	{
	public:
		TPSCamera(Window* window, Transform* target);
		~TPSCamera();

		void update(double dt) override;

		void onEvent(Window* window) override;
		void onKeyPressed(Direction dir) override;
		void onMouseMoved(glm::vec2& pos, bool constrain = true) override;
		void onMouseScrolled(glm::vec2& offset) override;
		void onMouseDown(int button) override;
		void onMouseUp(int button) override;

		void computePosition(float h_distance, float v_distance);
		float computeHorizontalDistance();
		float computeVerticalDistance();
		void computePitch();
		void computeAngle();
		void move();

	private:
		Transform* target;

		float sensitivity;
		float distance;
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
		
		float angle;
		float zoom_factor;
		float y_offset;

		glm::vec2 mouse_scroll;
		glm::vec2 mouse_position;
		bool mouse_left;
		bool mouse_right;
		bool mouse_middle;
	};

}


