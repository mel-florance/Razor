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
		sounds({}),
		device_infos(DeviceInfo())
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

		device_infos.renderer = (char*)alGetString(AL_RENDERER);
		device_infos.vendor = (char*)alGetString(AL_VENDOR);
		device_infos.version = (char*)alGetString(AL_VERSION);
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
			auto ext = filename.substr(filename.find_last_of(".") + 1);

			if (ext == "wav")
			{
				WAVData* wav_data = new WAVData();
				char* data = wav_loader->load(filename, wav_data);
				wav_data->data = data;

				Sound* sound = new Sound(short_name, wav_data);
				sounds[short_name] = sound;
			}
			else if (ext == "ogg")
			{
				OGGData* ogg_data = new OGGData();
				ogg_loader->load(filename, ogg_data);

				Sound* sound = new Sound(short_name, ogg_data);
				sounds[short_name] = sound;
			}
		}
	}

	void SoundsManager::playSound(const std::string& short_name)
	{
		auto it = sounds.find(short_name);

		if (it != sounds.end())
			it->second->play();
	}

}
