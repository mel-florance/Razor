#pragma once

#include "Camera.h"

namespace Razor {

	class TPSCamera : public Camera
	{
	public:
		TPSCamera(Window* window);
		~TPSCamera();

		void update(double dt) override;

		void onEvent(Window* window) override;
		void onKeyPressed(Direction dir) override;
		void onMouseMoved(glm::vec2& pos, bool constrain = true) override;
		void onMouseScrolled(glm::vec2& offset) override;
		void onMouseDown(int button) override;
		void onMouseUp(int button) override;
	};

}


