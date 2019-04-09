#pragma once

#include "TextureAttachment.h"
#include "RenderBuffer.h"

namespace Razor {

	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void bind() const;
		void unbind() const;
		TextureAttachment* addTextureAttachment(const glm::vec2& size);
		RenderBuffer* addRenderBufferAttachment(const glm::vec2& size);

	private:
		unsigned int id;
		std::vector<TextureAttachment*> texturesAttachments;
		std::vector<RenderBuffer*> rendersBuffersAttachments;
	};

}
