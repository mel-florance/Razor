#pragma once

#include "Razor/Core/Core.h"

namespace Razor
{

	class CubemapTexture
	{
	public:
		CubemapTexture(const std::vector<std::string>& textures);
		~CubemapTexture();

		inline unsigned int getId() { return id; }
		CubemapTexture* load();
		void bind(unsigned int unit);
		void unbind();

	private:
		unsigned int id;
		std::vector<std::string> textures;
	};

}
