#pragma once

namespace Razor
{
	class Window;

	class Viewport
	{
	public:
		Viewport(Window* window, const glm::vec2& position = glm::vec2(0.0f), const glm::vec2& size = glm::vec2(1920.0f, 1080.0f));
		~Viewport();

		inline glm::vec2& getPosition() { return position; }
		inline glm::vec2& getSize() { return size; }
		inline bool isHovered() { return hovered; }

		inline void setPosition(const glm::vec2& position) { this->position = position; }
		inline void setSize(const glm::vec2& size) { this->size = size; }
		inline void setHovered(bool value) { hovered = value; }

		inline bool& isVisible() { return visible; }

	private:
		Window* window;

		bool visible;
		bool hovered;
		glm::vec2 size;
		glm::vec2 position;
	};

}
