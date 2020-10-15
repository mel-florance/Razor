#pragma once

namespace Razor
{

	class GBuffer
	{
	public:
		GBuffer(const glm::ivec2& size);
		~GBuffer();

		inline unsigned int getBuffer() { return buffer; }
		inline unsigned int getPosition() { return position; }
		inline unsigned int getNormal() { return normal; }
		inline unsigned int getColor() { return color; }
		inline unsigned int getDepth() { return depth; }
		inline unsigned int getFrame() { return frame; }
		inline unsigned int getCombined() { return combined; }
		
	private:
		glm::ivec2 size;
		
		unsigned int buffer;
		unsigned int position;
		unsigned int normal;
		unsigned int color;
		unsigned int depth;

		unsigned int frame;
		unsigned int frame_depth;
		unsigned int combined;
	};

}
