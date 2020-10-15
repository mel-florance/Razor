#include "rzpch.h"
#include "WAVLoader.h"
#include "Razor/Filesystem/BinaryReader.h"

namespace Razor
{

	WAVLoader::WAVLoader()
	{
		reader = new BinaryReader();
	}

	WAVLoader::~WAVLoader()
	{
		delete reader;
	}

	char* WAVLoader::load(const std::string& filename, WAVData* data)
	{
		char buffer[4];
		std::ifstream in(filename, std::ios::binary);
		
		in.read(buffer, 4); // RIFF identifier
		if (strncmp(buffer, "RIFF", 4) != 0) {
			Log::error("Can't load wav file \"%s\", header missing RIFF identifier.", filename.c_str());
			return NULL;
		}
		 
		in.read(buffer, 4); // File size
		data->file_size = BinaryReader::readInt(buffer, 4);
	
		in.read(buffer, 4); // WAVE identifier
		if (strncmp(buffer, "WAVE", 4) != 0) {
			Log::error("Can't load wav file \"%s\", header missing WAVE identifier.", filename.c_str());
			return NULL;
		}
		
		in.read(buffer, 4); // fmt identifier
		if (strncmp(buffer, "fmt ", 4) != 0) {
			Log::error("Can't load wav file \"%s\", header missing fmt identifier.", filename.c_str());
			return NULL;
		}

		in.read(buffer, 4); // Bloc size
		data->bloc_size = BinaryReader::readInt(buffer, 4);

		in.read(buffer, 2); // Format
		data->format = BinaryReader::readInt(buffer, 2);

		in.read(buffer, 2); // Channels
		data->channels = BinaryReader::readInt(buffer, 2);

		in.read(buffer, 4); // Sample rate
		data->sample_rate = BinaryReader::readInt(buffer, 4);

		in.read(buffer, 4); // Byte rate
		data->byte_rate = BinaryReader::readInt(buffer, 4);

		in.read(buffer, 2); // Block align
		data->block_align = BinaryReader::readInt(buffer, 2);

		in.read(buffer, 2); // Bits per sample
		data->bits_per_sample = BinaryReader::readInt(buffer, 2);

		in.read(buffer, 4); // Subchunk id
		data->subchunk_id = BinaryReader::readInt(buffer, 4);

		in.read(buffer, 4); // Subchunk size
		data->subchunk_size = BinaryReader::readInt(buffer, 4);

		char* wav_data = new char[data->subchunk_size];
		in.read(wav_data, data->subchunk_size); // Data bloc

		in.close();

		Log::info("Loaded audio file: %s (%s)", filename.c_str(), Utils::bytesToSize(data->file_size).c_str());

		const char* str = "Wav data:\n"
			"\tFile size\t%s\n"
			"\tBloc size\t%d\n"
			"\tFormat\t\t%s\n"
			"\tChannels\t%d\n"
			"\tSample rate\t%d\n"
			"\tByte rate\t%d\n"
			"\tBlock align\t%d\n"
			"\tBits per sample\t%d\n"
			"\tSubchunk id\t%d\n"
			"\tSubchunk size\t%d\n";

		Log::info(str,
			Utils::bytesToSize(data->file_size).c_str(), 
			data->bloc_size, 
			compressionCodeToStr(data->format).c_str(), 
			data->channels, 
			data->sample_rate, 
			data->byte_rate, 
			data->block_align, 
			data->bits_per_sample,
			data->subchunk_id,
			data->subchunk_size);

		return wav_data;

	}

}
