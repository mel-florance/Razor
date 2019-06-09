#include "rzpch.h"
#include "VertexBuffer.h"
#include "glad/glad.h"

namespace Razor {

	VertexBuffer::VertexBuffer(const void* data, unsigned int size) :
		size(size),
		usage(BufferUsage::STATIC_DRAW)
	{
		glGenBuffers(1, &id);
		update(size, data);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::update(unsigned int size, const void* data)
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
	}

}
