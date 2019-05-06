#pragma once

namespace Razor
{

	class FrameBuffer;
	class TextureAttachment;

	class ShadowGenerator
	{
	public:
		ShadowGenerator(const glm::vec2& size = glm::vec2(1024.0f));
		~ShadowGenerator();

	private:
		glm::vec2 size;

		FrameBuffer* depth_buffer;
		TextureAttachment* depth_attachment;
	};

}
