#include "rzpch.h"
#include "OGGLoader.h"

#include "stb_vorbis.c"

namespace Razor
{

	OGGLoader::OGGLoader()
	{

	}

	OGGLoader::~OGGLoader()
	{
	}

	void OGGLoader::close()
	{
		/*alDeleteSources(1, &data->source_id);
		alDeleteBuffers(2, data->buffers);
		stb_vorbis_close(data->stream);
		delete data;*/
	}

	bool OGGLoader::stream(ALuint buffer)
	{
		return false;
	}

	void OGGLoader::load(const std::string& filename, OGGData* data)
	{
		stb_vorbis* ogg_data;
		stb_vorbis_info infos;

		ogg_data = stb_vorbis_open_filename(filename.c_str(), 0, 0);

		if (!ogg_data)
			return;

		infos = stb_vorbis_get_info(ogg_data);

		data->infos.channels                   = infos.channels;
		data->infos.max_frame_size             = infos.max_frame_size;
		data->infos.sample_rate                = infos.sample_rate;
		data->infos.setup_memory_required      = infos.setup_memory_required;
		data->infos.setup_temp_memory_required = infos.setup_temp_memory_required;
		data->infos.temp_memory_required       = infos.temp_memory_required;

		data->format = infos.channels == 2
			? AL_FORMAT_STEREO16
			: AL_FORMAT_MONO16;

		//alSourceQueueBuffers(data->source_id, 2, data->buffers);
		//alSourcePlay(data->source_id);

		data->buffer_size = stb_vorbis_stream_length_in_samples(ogg_data) * infos.channels;
		data->buffer = (int16*)malloc(data->buffer_size * sizeof(uint16));

		stb_vorbis_get_samples_short_interleaved(ogg_data, infos.channels, (short*)data->buffer, data->buffer_size);

		alGenBuffers(1, &data->source_id);
		alBufferData(data->source_id, data->format, data->buffer, data->buffer_size * sizeof(int16), data->infos.sample_rate);

		free(data->buffer);
		stb_vorbis_close(ogg_data);
	}

}
