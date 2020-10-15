#pragma once

#include <glm/glm.hpp>

namespace Razor
{

	class TextureAttachment
	{
	public:
		TextureAttachment(const glm::vec2& size = glm::vec2(256.0f, 256.0f), bool depth = false, bool transparent = false);
		~TextureAttachment();

		inline unsigned int& getId() { return id; }
		inline glm::vec2& getSize() { return size; }

		void bind(int slot = 0) const;
		void unbind() const;

		void resize(const glm::vec2& new_size);

	private:
		unsigned int id;
		glm::vec2 size;
		bool depth;
		bool transparent;
	};

}
