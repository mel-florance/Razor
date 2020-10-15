#pragma once

#include "Razor/Core/Core.h"
#include "VertexBuffer.h"

namespace Razor {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		enum BufferType 
		{
			BYTE           = 0x1400,
			UNSIGNED_BYTE  = 0x1401,
			SHORT          = 0x1402,
			UNSIGNED_SHORT = 0x1403,
			INT            = 0x1404,
			UNSIGNED_INT   = 0x1405,
			FLOAT          = 0x1406,
			DOUBLE         = 0x140A
		};

		void addBuffer(
			const VertexBuffer& vb,
			unsigned int attr_num, 
			unsigned int el_count,
			BufferType buffer_type = BufferType::FLOAT, 
			bool normalized = false,
			unsigned int stride = 0,
			void* offset = 0
		);

		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
	};

}
