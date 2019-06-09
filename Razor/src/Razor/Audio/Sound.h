#pragma once

#include "Razor/Audio/Loaders/WAVLoader.h"

namespace Razor
{

	class Sound
	{
	public:
		Sound(const std::string& name, char* data, const WAVLoader::WAVData& wav_data);
		~Sound();

		enum State
		{
			INITIAL = 0x1011,
			PLAYING = 0x1012,
			PAUSED  = 0x1013,
			STOPPED = 0x1014
		};

		enum Format
		{
			MONO_8    = 0x1100,
			MONO_16   = 0x1101,
			STEREO_8  = 0x1102,
			STEREO_16 = 0x1103
		};

		void play();
		void pause();
		void unpause();
		void stop();
		void bind();
		void unbind();

		inline std::string& getName() { return name; }
		inline State getState() { return state; }
		inline bool isLooping() { return looping; }
		bool isPlaying();
		inline float& getGain() { return gain; }
		inline float& getGainMin() { return gain_min; }
		inline float& getGainMax() { return gain_max; }
		inline float& getPitch() { return pitch; }
		inline float& getInnerAngle() { return inner_angle; }
		inline float& getOuterAngle() { return outer_angle; }
		inline glm::vec3& getPosition() { return position; }
		inline glm::vec3& getDirection() { return direction; }
		inline glm::vec3& getVelocity() { return velocity; }

		inline void setName(const std::string& name) { this->name = name; }
		inline void setState(State state) { this->state = state; }
		void setIsLooping(bool value);
		void setGain(float value);
		void setGainMin(float value);
		void setGainMax(float value);
		void setPitch(float value);
		void setInnerAngle(float value);
		void setOuterAngle(float value);
		void setPosition(const glm::vec3& position);
		void setDirection(const glm::vec3& direction);
		void setVelocity(const glm::vec3& velocity);

	private:
		unsigned int id;
		unsigned int format;
		unsigned int source_id;
		std::string name;
		char* data;

		State state;
		bool looping;
		float gain;
		float gain_min;
		float gain_max;
		float pitch;
		float inner_angle;
		float outer_angle;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 velocity;
	};

}
