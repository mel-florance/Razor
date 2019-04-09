#pragma once

namespace Razor {

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
		unsigned int size;
	};

}
