#pragma once

#include <glm/glm.hpp>

namespace Razor {

	class TextureAttachment
	{
	public:
		TextureAttachment(const glm::vec2& size);
		~TextureAttachment();

		inline unsigned int getId() { return id; }
		inline glm::vec2& getSize() { return size; }

		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
		glm::vec2 size;
	};

}
