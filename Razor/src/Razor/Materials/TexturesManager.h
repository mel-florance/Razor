#pragma once

namespace Razor
{
	class Texture;

	class TexturesManager
	{
	public:
		TexturesManager();
		~TexturesManager();

		bool hasTexture(const std::string& path);
		void addTexture(const std::string& path, Texture* texture);
		bool removeTexture(const std::string& path);
		Texture* getTexture(const std::string& path);

	private:
		std::map<std::string, Texture*> textures;
	};

}
