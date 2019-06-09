#include "rzpch.h"
#include "VertexArray.h"
#include <glad/glad.h>

namespace Razor {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}

	void VertexArray::addBuffer(
		const VertexBuffer& vb,
		unsigned int attr_num, 
		unsigned int el_count, 
		BufferType buffer_type, 
		bool normalized, 
		unsigned int stride,
		void* offset)
	{
		bind();
		vb.bind();
		glEnableVertexAttribArray(attr_num);
		glVertexAttribPointer(attr_num, el_count, buffer_type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
		vb.unbind();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

}
