#include "rzpch.h"
#include "ShadowGenerator.h"
#include "Razor/Buffers/FrameBuffer.h"
#include "Razor/Buffers/TextureAttachment.h"

namespace Razor
{

	ShadowGenerator::ShadowGenerator(const glm::vec2& size) : 
		size(size)
	{
		depth_buffer = new FrameBuffer();
		depth_attachment = depth_buffer->addTextureAttachment(size, true);
	}

	ShadowGenerator::~ShadowGenerator()
	{
		delete depth_buffer;
	}

}
