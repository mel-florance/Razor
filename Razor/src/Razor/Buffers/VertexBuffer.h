#pragma once

namespace Razor {

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		enum class BufferUsage
		{
			STREAM_DRAW  = 0x88E0,
			STREAM_READ  = 0x88E1,
			STREAM_COPY  = 0x88E2,
			STATIC_DRAW  = 0x88E4,
			STATIC_READ  = 0x88E5,
			STATIC_COPY  = 0x88E6,
			DYNAMIC_DRAW = 0x88E8,
			DYNAMIC_READ = 0x88E9,
			DYNAMIC_COPY = 0x88EA
		};

		void bind() const;
		void unbind() const;
		void update(unsigned int size, const void* data);

		inline BufferUsage getUsage() { return usage; }
		inline void setUsage(BufferUsage usage) { this->usage = usage; }

	private:
		unsigned int id;
		unsigned int size;
		BufferUsage usage;
	};

}
