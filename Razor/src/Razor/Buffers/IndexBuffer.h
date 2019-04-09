#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const { return count; }

	private:
		unsigned int id;
		unsigned int count;
	};

}
