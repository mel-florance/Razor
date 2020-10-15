#pragma once

#include "Camera.h"

namespace Razor
{

	class Viewport;
	class Transform;

	class TPSCamera : public Camera
	{
	public:
		TPSCamera(Window* window, TPSCamera* cam);
		TPSCamera(Window* window);

		virtual ~TPSCamera();

		virtual void update(double dt) override;
		virtual void setProjection();
		void setTarget(Transform* transform);
		inline Transform* getTarget() { return target; }

		inline void setPitch(float value) { pitch = value; }
		inline void setYaw(float value) { yaw = value; }
		inline void setRoll(float value) { roll = value; }
		inline void setAngle(float value) { angle = value; }
		inline void setDistance(float value) { distance = value; }

		inline float getAlpha() { return alpha; }
		inline float getPitch() { return pitch; }
		inline float getPitchMin() { return pitch_min; }
		inline float getPitchMax() { return pitch_max; }
		inline glm::vec2& getPitchOffset() { return pitch_offset; }
		inline float getPitchFactor() { return pitch_factor; }
		inline float getYaw() { return yaw; }
		inline float getRoll() { return roll; }
		inline float getDistance() { return distance; }
		inline float getDistanceMin() { return distance_min; }
		inline float getDistanceMax() { return distance_max; }
		inline float getAngle() { return angle; }
		inline float getSensitivity() { return sensitivity; }
		inline float getPanSensitivity() { return pan_sensitivity; }
		inline float getZoomFactor() { return zoom_factor; }
		inline float getYOffset() { return y_offset; }
		inline glm::vec3 getDeltaOffset() { return delta_offset; }

		virtual void onMouseMoved(glm::vec2& pos, bool constrain = true) override;
		virtual void onMouseScrolled(glm::vec2& offset) override;
		virtual void onMouseDown(int button) override;
		virtual void onMouseUp(int button) override;
		virtual void onKeyDown(int keyCode) override;
		virtual void onKeyReleased(int keyCode) override;
		virtual void onWindowResized(const glm::vec2& size) override;

	private:
		Transform* target;

		float distance;
		float distance_smooth;
		float distance_min;
		float distance_max;

		float pitch;
		float pitch_min;
		float pitch_max;
		float pitch_factor;

		float yaw;
		float yaw_min;
		float yaw_max;

		float roll;
		float roll_min;
		float roll_max;
		
		float alpha;
		float angle;
		float y_offset;
		float sensitivity;
		float pan_sensitivity;

		float zoom;
		float zoom_factor;
		float zoom_lerp;

		glm::vec3 delta_offset;
		glm::vec2 pitch_offset;

		glm::vec2 mouse_scroll;
		glm::vec2 mouse_position;
	};

}
