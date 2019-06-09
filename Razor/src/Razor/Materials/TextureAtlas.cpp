#include "rzpch.h"
#include "TextureAtlas.h"

namespace Razor
{

	TextureAtlas::TextureAtlas(const std::string& filename, bool mipmaps, bool flipped) :
		Texture(filename, mipmaps, flipped),
		min(glm::vec2(6, 6)),
		max(glm::vec2(78, 82)),
		size(glm::vec2(88, 88))
	{
	}

	TextureAtlas::~TextureAtlas()
	{
	}

}
