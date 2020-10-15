#pragma once

#include "Razor/Assets/Asset.h"

namespace Razor
{
	class Sound;

	class AudioAsset : public Asset
	{
	public:
		AudioAsset(const std::string& path);
		~AudioAsset() override;

		void load() override;
		void serialize() override;
		void unserialize() override;

	private:
		std::shared_ptr<Sound> data;
	};

}
