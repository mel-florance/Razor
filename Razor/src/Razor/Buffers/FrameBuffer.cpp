#include "rzpch.h"
#include "Razor/Core/Engine.h"
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

	int FrameBuffer::getSlot(int slot)
	{
		return GL_COLOR_ATTACHMENT0 + slot;
	}

	void FrameBuffer::blit(int width, int height)
	{
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void FrameBuffer::bind(BindMode mode, int buffer_id) const
	{
		glBindFramebuffer(mode, buffer_id);
	}

	void FrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TextureAttachment* FrameBuffer::addTextureAttachment(const glm::vec2& size, bool depth, bool transparent, int slot)
	{
		TextureAttachment* texture = new TextureAttachment(size, depth, transparent);
		bind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT : getSlot(slot), GL_TEXTURE_2D, texture->getId(), 0);
		

		texturesAttachments.push_back(texture);

		return texture;
	}

	void FrameBuffer::updateTextureAttachment(bool depth)
	{
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturesAttachments[0]->getId(), 0);
	}

	RenderBuffer* FrameBuffer::addRenderBufferAttachment(const glm::vec2& size)
	{
		RenderBuffer* buffer = new RenderBuffer(size);
		bind();
	
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->getId());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		rendersBuffersAttachments.push_back(buffer);

		unbind();

		return buffer;
	}

	void FrameBuffer::setAttachmentUsage(std::vector<unsigned int> attachments)
	{
		glDrawBuffers(attachments.size(), &attachments[0]);
	}

}
