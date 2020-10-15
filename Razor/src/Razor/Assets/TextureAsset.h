#pragma once

#include "Razor/Assets/Asset.h"

namespace Razor
{
	class Texture;

	class TextureAsset : public Asset
	{
	public:
		TextureAsset(const std::string& path);
		~TextureAsset() override;

		void load() override;
		void serialize() override;
		void unserialize() override;

	private:
		std::shared_ptr<Texture> data;
	};

}
