#pragma once

namespace Razor
{

	class BinaryReader
	{
	public:
		BinaryReader();
		~BinaryReader();

		inline static bool isBigEndian()
		{
			int a = 1;
			return !((char*)& a)[0];
		}

		inline static int readInt(char* buffer, int length)
		{
			int a = 0;

			if (!isBigEndian())
				for (int i = 0; i < length; ++i)
					((char*)& a)[i] = buffer[i];
			else
				for (int i = 0; i < length; ++i)
					((char*)& a)[3 - i] = buffer[i];

			return a;
		}

	};

}
