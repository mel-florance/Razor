#include "rzpch.h"
#include "TextureAttachment.h"
#include <glad/glad.h>

namespace Razor {

	TextureAttachment::TextureAttachment(const glm::vec2& size, bool depth, bool transparent) :
		size(size), 
		depth(depth),
		transparent(transparent)
	{
		glGenTextures(1, &id);
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (depth)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		int format = transparent ? GL_RGBA : GL_RGB;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			depth ? GL_DEPTH_COMPONENT24 : format,
			(GLsizei)size.x,
			(GLsizei)size.y,
			0,
			depth ? GL_DEPTH_COMPONENT : format,
			transparent ? GL_UNSIGNED_BYTE : GL_FLOAT,
			NULL
		);
	}

	TextureAttachment::~TextureAttachment()
	{
		glDeleteTextures(1, &id);
	}

	void TextureAttachment::bind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void TextureAttachment::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextureAttachment::resize(const glm::vec2& new_size)
	{
		int format = transparent ? GL_RGBA : GL_RGB;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			depth ? GL_DEPTH_COMPONENT24 : format,
			(GLsizei)new_size.x,
			(GLsizei)new_size.y,
			0,
			depth ? GL_DEPTH_COMPONENT : format,
			GL_UNSIGNED_BYTE,
			NULL
		);
	}
}

