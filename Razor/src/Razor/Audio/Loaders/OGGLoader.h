#pragma once
#include "Razor/Core/Core.h"
#include "Razor/Audio/Audio.h"

namespace Razor
{
	
	class OGGLoader
	{
	public:
		OGGLoader();
		~OGGLoader();

		void close();
		bool stream(ALuint buffer);
		void load(const std::string& filename, OGGData* data);
	};

}
