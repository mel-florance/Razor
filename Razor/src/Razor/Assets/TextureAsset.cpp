#include "rzpch.h"
#include "TextureAsset.h"
#include "Razor/Materials/Texture.h"

namespace Razor
{

	TextureAsset::TextureAsset(const std::string& path) :
		Asset(path),
		data(nullptr)
	{
	}

	TextureAsset::~TextureAsset()
	{
	}

	void TextureAsset::load()
	{
	}

	void TextureAsset::serialize()
	{
	}

	void TextureAsset::unserialize()
	{
	}

}