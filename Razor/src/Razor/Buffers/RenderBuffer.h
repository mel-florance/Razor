#pragma once

#include <glm/glm.hpp>

namespace Razor {

	class RenderBuffer
	{
	public:
		RenderBuffer(const glm::vec2& size);
		~RenderBuffer();

		inline unsigned int getId() { return id;  }

		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
		glm::vec2 size;
	};

}
