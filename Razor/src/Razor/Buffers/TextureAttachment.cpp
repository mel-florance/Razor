#include "rzpch.h"
#include "TextureAttachment.h"
#include <glad/glad.h>

namespace Razor {

	TextureAttachment::TextureAttachment(const glm::vec2& size, bool depth) :
		size(size), 
		depth(depth)
	{
		glGenTextures(1, &id);
		bind();

		glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT32 : GL_RGB, (GLsizei)size.x, (GLsizei)size.y, 0, depth ? GL_DEPTH_COMPONENT : GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (depth)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
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

