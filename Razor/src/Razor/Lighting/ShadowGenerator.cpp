#include "rzpch.h"
#include "ShadowGenerator.h"
#include "Razor/Buffers/FrameBuffer.h"
#include "Razor/Buffers/TextureAttachment.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Razor
{

	ShadowGenerator::ShadowGenerator(const glm::vec2& size) : 
		size(size),
		clip_plane(glm::vec2(0.0f, 35.0f))
	{
		depth_buffer = new FrameBuffer();
		depth_attachment = depth_buffer->addTextureAttachment(size, true);

		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, clip_plane.x, clip_plane.y);
	}

	ShadowGenerator::~ShadowGenerator()
	{
		delete depth_buffer;
	}

}
