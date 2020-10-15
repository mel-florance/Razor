#include "rzpch.h"
#include "AudioAsset.h"
#include "Razor/Audio/Sound.h"
#include "Razor/Audio/Loaders/WAVLoader.h"

namespace Razor
{

	AudioAsset::AudioAsset(const std::string& path) :
		Asset(path),
		data(nullptr)
	{
	}

	AudioAsset::~AudioAsset()
	{
	}

	void AudioAsset::load()
	{
		//WAVData* wav_data = new WAVData();
		//char* bytes = WAVLoader::load(path, wav_data);
		//wav_data->data = bytes;

		//data = std::make_shared<Sound>("", bytes, wav_data);
	}

	void AudioAsset::serialize()
	{
	}

	void AudioAsset::unserialize()
	{
	}

}
