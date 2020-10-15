#include "rzpch.h"
#include "RenderBuffer.h"
#include <glad/glad.h>

namespace Razor {

	RenderBuffer::RenderBuffer(const glm::vec2& size) : size(size)
	{
		glGenRenderbuffers(1, &id);
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)size.x, (GLsizei)size.y);
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &id);
	}

	void RenderBuffer::bind() const 
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}

	void RenderBuffer::unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	};
}

