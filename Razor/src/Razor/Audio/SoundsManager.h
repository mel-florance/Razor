#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace Razor
{

	class Sound;
	class WAVLoader;
	class OGGLoader;

	class SoundsManager
	{
	public:
		SoundsManager();
		~SoundsManager();

		enum DistanceModel
		{
			LINEAR,
			EXP,
			INVERSE,
			LINEAR_CLAMPED,
			EXP_CLAMPED,
			INVERSE_CLAMPED
		};

		void loadSound(const std::string& filename, const std::string& short_name);
		void playSound(const std::string& short_name);
		void setSound(const std::string& short_name, Sound* sound) { sounds[short_name] = sound; }
		bool hasSound(const std::string& short_name) { return sounds.find(short_name) != sounds.end(); }

		inline DistanceModel getDistanceModel() { return distance_model; }
		inline ALCdevice* getDevice() { return device; }
		inline ALCcontext* getContext() { return context; }
		inline WAVLoader* getWavLoader() { return wav_loader; }
		inline OGGLoader* getOggLoader() { return ogg_loader; }
		inline std::map<std::string, Sound*>& getSounds() { return sounds; }

		inline void setDistanceModel(DistanceModel model) { distance_model = model; }
		inline void setDevice(ALCdevice* device) { this->device = device; }
		inline void setContext(ALCcontext* context) { this->context = context; }
		inline void setWavLoader(WAVLoader* loader) { this->wav_loader = loader; }
		inline void setOggLoader(OGGLoader* loader) { this->ogg_loader = loader; }
		inline void setSounds(const std::map<std::string, Sound*>& sounds) { this->sounds = sounds; }

	private:
		DistanceModel distance_model;
		ALCdevice* device;
		ALCcontext* context;
		WAVLoader* wav_loader;
		OGGLoader* ogg_loader;
		std::map<std::string, Sound*> sounds;
	};

}
