#pragma once

#include "Razor/Assets/Asset.h"

namespace Razor
{

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		enum AssetType 
		{ 
			MESH, 
			TEXTURE,
			AUDIO
		};

		inline void addAsset(std::shared_ptr<Asset> asset) { assets.push_back(asset); }

	private:
		std::vector<std::shared_ptr<Asset>> assets;
	};

}
