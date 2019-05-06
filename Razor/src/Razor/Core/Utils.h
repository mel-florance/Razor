#pragma once

#include "rzpch.h"
#include "Core.h"

namespace Razor 
{

	class Utils
	{
	public:

		static bool treeNode(const char* label);

		static inline std::string& ltrim(std::string& str)
		{
			size_t start = str.find_first_not_of(" \t\r\n\v\f");
		
			if (std::string::npos != start)
				str = str.substr(start);

			return str;
		}

		static inline std::string& rtrim(std::string& str)
		{
			size_t end = str.find_last_not_of(" \t\r\n\v\f");

			if (std::string::npos != end)
				str = str.substr(0, end + 1);

			return str;
		}

		static inline std::vector<std::string> splitPath(const std::string& str, const std::set<char> delimiters)
		{
			std::vector<std::string> result;

			char const* pch = str.c_str();
			char const* start = pch;

			for (; *pch; ++pch) 
			{
				if (delimiters.find(*pch) != delimiters.end()) 
				{
					if (start != pch)
						std::string str(start, pch);
					else
						result.push_back("");

					start = pch + 1;
				}
			}

			result.push_back(start);

			return result;
		}

		static inline std::vector<std::string> splitString(const std::string& str, const std::string& delimiter)
		{
			std::vector<std::string> strings;
			std::string::size_type pos = 0;
			std::string::size_type prev = 0;

			while ((pos = str.find(delimiter, prev)) != std::string::npos)
			{
				strings.push_back(str.substr(prev, pos - prev));
				prev = pos + 1;
			}

			strings.push_back(str.substr(prev));

			return strings;
		}

		static inline std::string getFileSize(const std::string& filename) 
		{
			FILE *p_file = NULL;
			p_file = fopen(filename.c_str(), "rb");
			int size = ftell(p_file);
			fseek(p_file, 0, SEEK_END);
			fclose(p_file);

			const char *sizes[5] = { "bytes", "Kb", "Mb", "Gb", "Tb" };

			int i;
			double dblByte = size;

			for (i = 0; i < 5 && size >= 1024; i++, size /= 1024)
				dblByte = size / 1024.0;

			return std::to_string(dblByte) + " " + std::string(sizes[i]);
		};

		static inline std::string remove_extension(const std::string& filename)
		{
			size_t lastdot = filename.find_last_of(".");
			if (lastdot == std::string::npos) return filename;
			return filename.substr(0, lastdot);
		}

		static void gen_random(char* str, const int len)
		{
			static const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";

			for (int i = 0; i < len; ++i)
				str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

			str[len] = 0;
		}

		static float randomf(float a, float b)
		{
			float random = ((float)rand()) / (float)RAND_MAX;
			float diff = b - a;
			float r = random * diff;

			return a + r;
		}

		static inline float mapValues(float rangeA[], float rangeB[], float value, unsigned int size = 2)
		{
			return (value - rangeA[0]) * (rangeB[1] - rangeB[0]) / (rangeA[1] - rangeA[0]) + rangeB[0];
		}

		static char* cpToUTF8(int cp, char* str)
		{
			int n = 0;

			if (cp < 0x80) n = 1;
			else if (cp < 0x800) n = 2;
			else if (cp < 0x10000) n = 3;
			else if (cp < 0x200000) n = 4;
			else if (cp < 0x4000000) n = 5;
			else if (cp <= 0x7fffffff) n = 6;

			str[n] = '\0';

			switch (n) {
				case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
				case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
				case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
				case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
				case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
				case 1: str[0] = cp;
			}

			return str;
		}

		template <typename F, typename T>
		static void copyMatrix(F& from, T& to)
		{
			to[0][0] = from.a1; to[1][0] = from.a2;
			to[2][0] = from.a3; to[3][0] = from.a4;
			to[0][1] = from.b1; to[1][1] = from.b2;
			to[2][1] = from.b3; to[3][1] = from.b4;
			to[0][2] = from.c1; to[1][2] = from.c2;
			to[2][2] = from.c3; to[3][2] = from.c4;
			to[0][3] = from.d1; to[1][3] = from.d2;
			to[2][3] = from.d3; to[3][3] = from.d4;
		}

		
	};

}
