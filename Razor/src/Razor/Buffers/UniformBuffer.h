#pragma once

namespace Razor {

	class UniformBuffer
	{
	public:
		UniformBuffer(const void* data);
		~UniformBuffer();

		void link(unsigned int program);
		void update(const void* data);
		void bind() const;
		void unbind() const;

	private:
		unsigned int id;
	};

}
