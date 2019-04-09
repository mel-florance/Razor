#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	struct VertexBufferElement
	{
		unsigned int  count;
		unsigned int  type;
		unsigned char normalized;

		static unsigned int getTypeSize(unsigned int type);
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		~VertexBufferLayout();

		void pushFloat(unsigned int count);
		void pushUint(unsigned int count);
		void pushUchar(unsigned int count);

		inline const std::vector<VertexBufferElement> getElements() const { return elements; }
		inline unsigned int getStride() const { return stride; }

	private:
		unsigned int stride;
		std::vector<VertexBufferElement> elements;
	};

}
