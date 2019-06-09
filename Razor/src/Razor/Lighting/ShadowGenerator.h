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

		inline void setViewMatrix(const glm::mat4& matrix) { view = matrix; }
		inline void setProjectionMatrix(const glm::mat4& matrix) { projection = matrix; }

		inline glm::mat4& getViewMatrix() { return view; }
		inline glm::mat4& getProjectionMatrix() { return projection; }
		inline glm::vec2& getSize() { return size; }
		inline FrameBuffer* getDepthBuffer() { return depth_buffer; }
		inline TextureAttachment* getDepthAttachment() { return depth_attachment; }

	private:
		glm::vec2 size;

		FrameBuffer* depth_buffer;
		TextureAttachment* depth_attachment;

		glm::mat4 view;
		glm::mat4 projection;

		glm::vec2 clip_plane;
	};

}
