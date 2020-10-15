#pragma once

#include "TextureAttachment.h"
#include "RenderBuffer.h"

namespace Razor {

	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		static int getSlot(int slot);

		enum BindMode {
			DEFAULT = 0x8D40,
			READ = 0x8CA8,
			DRAW = 0x8CA9
		};

		void blit(int width, int height);
		void bind(BindMode mode = BindMode::DEFAULT, int buffer_id = 0) const;
		void unbind() const;
		TextureAttachment* addTextureAttachment(const glm::vec2& size, bool depth = false, bool transparent = false, int slot = 0);
		void updateTextureAttachment(bool depth = false);
		RenderBuffer* addRenderBufferAttachment(const glm::vec2& size);
		inline unsigned int getId() { return id; }

		void setAttachmentUsage(std::vector<unsigned int> attachments);

	private:
		unsigned int id;
		std::vector<TextureAttachment*> texturesAttachments;
		std::vector<RenderBuffer*> rendersBuffersAttachments;
	};

}
