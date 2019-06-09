#include "rzpch.h"
#include "SoundsManager.h"
#include "Razor/Audio/Sound.h"
#include "Razor/Audio/Loaders/WAVLoader.h"
#include "Razor/Audio/Loaders/OGGLoader.h"

namespace Razor
{

	SoundsManager::SoundsManager() :
		context(nullptr),
		device(nullptr),
		wav_loader(nullptr),
		ogg_loader(nullptr),
		distance_model(DistanceModel::LINEAR),
		sounds({})
	{
		wav_loader = new WAVLoader();
		ogg_loader = new OGGLoader();

		device = alcOpenDevice(NULL);

		if (device == NULL) {
			Log::error("Cannot open sound card.");
			return;
		}

		context = alcCreateContext(device, NULL);
			
		if (context == NULL) {
			Log::error("Cannot create OpenAL context.");
			return;
		}

		alcMakeContextCurrent(context);
	}

	SoundsManager::~SoundsManager()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);

		delete wav_loader;
		delete ogg_loader;
	}

	void SoundsManager::loadSound(const std::string& filename, const std::string& short_name)
	{
		auto item = sounds.find(short_name);

		if (item == sounds.end())
		{
			WAVLoader::WAVData wav_data;
			char* data = wav_loader->load(filename, wav_data);

			Sound* sound = new Sound(short_name, data, wav_data);
			sounds[short_name] = sound;
		}
	}

	void SoundsManager::playSound(const std::string& short_name)
	{
		auto it = sounds.find(short_name);

		if (it != sounds.end())
			it->second->play();
	}

}
