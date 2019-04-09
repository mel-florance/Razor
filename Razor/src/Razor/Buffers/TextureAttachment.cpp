#include "rzpch.h"
#include "TextureAttachment.h"
#include <glad/glad.h>

namespace Razor {

	TextureAttachment::TextureAttachment(const glm::vec2& size) : size(size)
	{
		glGenTextures(1, &id);
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	TextureAttachment::~TextureAttachment()
	{
		glDeleteTextures(1, &id);
	}

	void TextureAttachment::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void TextureAttachment::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

