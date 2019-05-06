#pragma once

namespace Razor {

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;
		void update(unsigned int size, const void* data);

	private:
		unsigned int id;
		unsigned int size;
	};

}
