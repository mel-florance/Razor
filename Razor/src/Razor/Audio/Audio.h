#pragma once

#include <AL/al.h>

namespace Razor
{

	class AudioData
	{
	public:
		AudioData() {}
		virtual ~AudioData() {}

		ALuint source_id;
	};

	struct OggInfos
	{
		int channels;
		int max_frame_size;
		unsigned int sample_rate;
		unsigned int setup_memory_required;
		unsigned int setup_temp_memory_required;
		unsigned int temp_memory_required;
	};

	class OGGData : public AudioData
	{
	public:
		OGGData() {}
		~OGGData() {}

		ALuint id;
		ALuint buffers[2];
		ALenum format;
		OggInfos infos;
		size_t buffer_size;
		size_t total_samples_left;
		bool looping;
		int16* buffer;
	};

	class WAVData : public AudioData
	{
	public:
		WAVData() {}
		~WAVData() {}

		int channels;
		int file_size;
		int sample_rate;
		int bits_per_sample;
		int bloc_size;
		int format;
		int byte_rate;
		int block_align;
		int subchunk_id;
		int subchunk_size;
		char* data;
	};

}
