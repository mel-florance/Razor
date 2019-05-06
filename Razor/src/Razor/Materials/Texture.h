#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class RAZOR_API Texture
	{
	public:
		Texture(const std::string& filename, bool mimaps = false, bool flipped = true);
		virtual ~Texture();

		inline unsigned int getId() { return id; }
		void setLoadBias(float& value) { lodBias = value; }
		float& getLodBias() { return lodBias; }
		Texture* load();
		void bind(unsigned int unit);
		void unbind();

	private:
		unsigned int id;
		std::string filename;
		bool mipmaps;
		float lodBias;
		bool flipped;
	};

}
