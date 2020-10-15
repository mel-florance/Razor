#include "rzpch.h"
#include "Sound.h"
#include "Audio.h"

namespace Razor
{

	Sound::Sound(const std::string& name, AudioData* audio_data) :
		id(NULL),
		format(NULL),
		name(name),
		audio_data(audio_data),
		state(State::INITIAL),
		looping(false),
		gain(1.0f),
		gain_min(1.0f),
		gain_max(1.0f),
		pitch(1.0f),
		inner_angle(360.0f),
		outer_angle(1.0f),
		position(glm::vec3(0.0f)),
		direction(glm::vec3(0.0f)),
		velocity(glm::vec3(0.0f)),
		is_audio_stream(false)
	{
		if (WAVData* wav_data = dynamic_cast<WAVData*>(audio_data))
		{
			if (wav_data->channels == 1)
				format = wav_data->bits_per_sample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
			else
				format = wav_data->bits_per_sample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

			alGenBuffers(1, &id);
			alBufferData(id, format, wav_data->data, wav_data->subchunk_size, wav_data->sample_rate);
			bind();
		}
		else if (OGGData* ogg_data = dynamic_cast<OGGData*>(audio_data))
		{
			is_audio_stream = true;

		}
	}

	Sound::~Sound()
	{
		unbind();
	}

	void Sound::play()
	{
		stop();
		alSourcePlay(audio_data->source_id);
	}

	void Sound::pause()
	{
		alSourcePause(audio_data->source_id);
	}

	void Sound::unpause()
	{
		alSourcePlay(audio_data->source_id);
	}

	void Sound::bind()
	{
		alGenSources(1, &audio_data->source_id);
		alSourcei(audio_data->source_id, AL_BUFFER, id);
	}

	void Sound::stop()
	{
		alSourceStop(audio_data->source_id);
	}

	void Sound::unbind()
	{
		stop();
		alDeleteSources(1, &audio_data->source_id);
		alDeleteBuffers(1, &id);
	}

	bool Sound::isPlaying()
	{
		ALint playing;
		alGetSourcei(audio_data->source_id, AL_SOURCE_STATE, &playing);

		return playing == AL_PLAYING;
	}

	void Sound::setIsLooping(bool value)
	{
		looping = value;
		alSourcei(audio_data->source_id, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}

	void Sound::setGain(float value)
	{
		gain = value;
		alSourcef(audio_data->source_id, AL_GAIN, gain);
	}

	void Sound::setGainMin(float value)
	{
		gain_min = value;
		alSourcef(audio_data->source_id, AL_MIN_GAIN, gain_min);
	}

	void Sound::setGainMax(float value)
	{
		gain_max = value;
		alSourcef(audio_data->source_id, AL_MAX_GAIN, gain_max);
	}

	void Sound::setPitch(float value)
	{
		pitch = value;
		alSourcef(audio_data->source_id, AL_PITCH, pitch);
	}

	void Sound::setInnerAngle(float value)
	{
		inner_angle = value;
		alSourcef(audio_data->source_id, AL_CONE_INNER_ANGLE, inner_angle);
	}

	void Sound::setOuterAngle(float value)
	{
		outer_angle = value;
		alSourcef(audio_data->source_id, AL_CONE_OUTER_ANGLE, outer_angle);
	}

	void Sound::setPosition(const glm::vec3& position)
	{
		this->position = position;
		alSource3f(audio_data->source_id, AL_POSITION, position.x, position.y, position.z);
	}

	void Sound::setDirection(const glm::vec3& direction)
	{
		this->direction = direction;
		alSource3f(audio_data->source_id, AL_DIRECTION, direction.x, direction.y, direction.z);
	}

	void Sound::setVelocity(const glm::vec3& velocity)
	{
		this->velocity = velocity;
		alSource3f(audio_data->source_id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

}
