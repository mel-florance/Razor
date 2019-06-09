#include "rzpch.h"
#include "TexturesManager.h"

namespace Razor
{

	TexturesManager::TexturesManager() : 
		textures({})
	{
	}

	TexturesManager::~TexturesManager()
	{
	}

	bool TexturesManager::hasTexture(const std::string & path)
	{
		return textures.find(path) != textures.end();
	}

	void TexturesManager::addTexture(const std::string& path, Texture* texture)
	{
		textures[path] = texture;
	}

	bool TexturesManager::removeTexture(const std::string& path)
	{
		return false;
	}

	Texture* TexturesManager::getTexture(const std::string & path)
	{
		if (hasTexture(path))
			return textures[path];

		return nullptr;
	}

}

