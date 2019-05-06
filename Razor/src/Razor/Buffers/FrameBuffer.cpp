#include "rzpch.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include <glad/glad.h>

namespace Razor {

	FrameBuffer::FrameBuffer()
	{
		glGenFramebuffers(1, &id);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &id);

		for (auto texture : texturesAttachments)
			delete texture;
	}

	void FrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void FrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TextureAttachment* FrameBuffer::addTextureAttachment(const glm::vec2& size, bool depth)
	{
		TextureAttachment* texture = new TextureAttachment(size, depth);
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
		texturesAttachments.push_back(texture);

		return texture;
	}

	RenderBuffer* FrameBuffer::addRenderBufferAttachment(const glm::vec2& size)
	{
		RenderBuffer* buffer = new RenderBuffer(size);
		bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->getId());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

		rendersBuffersAttachments.push_back(buffer);

		unbind();

		return buffer;
	}

}
