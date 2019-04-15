#pragma once

#include "Camera.h"

namespace Razor {

	class FPSCamera : public Camera
	{
	public:
		FPSCamera(Window* window);
		~FPSCamera();

		void updateVectors();
		void update(double dt) override;

		void onEvent(Window* window) override;
		void onKeyPressed(Direction dir) override;
		void onMouseMoved(glm::vec2& pos, bool constrain = true) override;
		void onMouseScrolled(glm::vec2& offset) override;
		void onMouseDown(int button) override;
		void onMouseUp(int button) override;
		void onWindowResized(const glm::vec2& size);



	private:
		float speed;
		float min_speed;
		float max_speed;
		float sensitivity;
	
		float view_friction;
		float move_friction;
	};

}
