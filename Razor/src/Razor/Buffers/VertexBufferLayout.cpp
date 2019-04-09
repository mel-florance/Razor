#include "rzpch.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

namespace Razor {

	VertexBufferLayout::VertexBufferLayout() : stride(0)
	{
	}

	VertexBufferLayout::~VertexBufferLayout()
	{
	}

	unsigned int VertexBufferElement::getTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT         : return 4;
			case GL_UNSIGNED_INT  : return 4;
			case GL_UNSIGNED_BYTE : return 1;
		}

		return 0;
	}

	void VertexBufferLayout::pushFloat(unsigned int count)
	{
		elements.push_back({ count, GL_FLOAT, GL_FALSE });
		stride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
	}

	void VertexBufferLayout::pushUint(unsigned int count)
	{
		elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
	}

	void VertexBufferLayout::pushUchar(unsigned int count)
	{
		elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
	}

}
