#include "rzpch.h"
#include "TextureAtlas.h"

namespace Razor
{

	TextureAtlas::TextureAtlas(const std::string& filename, bool mipmaps, bool flipped) :
		Texture(filename, mipmaps, flipped),
		offset(glm::vec2(3, 3)),
		size(glm::vec2(60, 60))
	{

	}

	TextureAtlas::~TextureAtlas()
	{
	}

}
