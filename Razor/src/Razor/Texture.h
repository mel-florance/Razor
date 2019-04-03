#pragma once

#include "Razor/Core.h"
#include "glad/glad.h"

namespace Razor {

	class RAZOR_API Texture
	{
	public:
		Texture(const std::string& filename, bool mimaps = false);
		virtual ~Texture();

		void setLoadBias(float& value) { lodBias = value; }
		float& getLodBias() { return lodBias; }
		Texture* load();
		void bind(unsigned int unit);
		void unbind();

	private:
		GLuint texture;
		std::string filename;
		bool mipmaps;
		float lodBias;
	};

}
