#pragma once

namespace Razor
{

	class BinaryReader;

	class WAVLoader
	{
	public:
		WAVLoader();
		~WAVLoader();

		enum CompressionCodes
		{
			Unknown               = 0x0000,
			PCM                   = 0x0001,
			MS_ADPCM              = 0x0002,
			ITU_G711_A_LOW        = 0x0006,
			ITU_G711_AU_LOW       = 0x0007,
			IMA_ADPCM             = 0x0011,
			ITU_G723_ADPCM_YAMAHA = 0x0016,
			GSM_610               = 0x0031,
			ITU_G721_ADPCM        = 0x0040,
			MPEG                  = 0x0050,
			EXPERIMENTAL          = 0xFFFF
		};

		inline std::string compressionCodeToStr(int code)
		{
			switch (code)
			{
				default:
				case Unknown:               return "Unknown"; break;
				case PCM:                   return "PCM / uncompressed"; break;
				case MS_ADPCM:              return "Microsoft ADPCM"; break;
				case ITU_G711_A_LOW:        return "ITU G.711 a-law"; break;
				case ITU_G711_AU_LOW:       return "ITU G.711 µ-law"; break;
				case IMA_ADPCM:             return "IMA ADPCM"; break;
				case ITU_G723_ADPCM_YAMAHA: return "ITU G.723 ADPCM (Yamaha)"; break;
				case GSM_610:               return "GSM 6.10"; break;
				case ITU_G721_ADPCM:        return "ITU G.721 ADPCM"; break;
				case MPEG:                  return "MPEG"; break;
				case EXPERIMENTAL:          return "Experimental"; break;
			}
		}

		struct WAVData
		{
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
		};

		char* load(const std::string& filename, WAVData& data);

	private:
		BinaryReader* reader;
	};

}
