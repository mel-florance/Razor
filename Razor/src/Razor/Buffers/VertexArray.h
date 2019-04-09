#pragma once

#include "Razor/Core/Core.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Razor {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
	};

}
