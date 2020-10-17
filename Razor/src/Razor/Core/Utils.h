#pragma once

#include "rzpch.h"
#include "Core.h"
#include <commdlg.h>

namespace Razor 
{

	class Utils
	{
	public:

		static bool treeNode(
			const char* label,
			bool has_icon = true,
			bool stripe = true,
			const std::string& icon_name = "arrow_button",
			const std::string& icon_hover = std::string(),
			bool selected = false
		);

		static void initColumns(float size);

		static inline glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float time) {
			return glm::vec4(
				a.x + (b.x - a.x) * time,
				a.y + (b.y - a.y) * time,
				a.z + (b.z - a.z) * time,
				a.w + (b.w - a.w) * time
			);
		}

		static inline glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float time) {
			return glm::vec3(
				a.x + (b.x - a.x) * time,
				a.y + (b.y - a.y) * time,
				a.z + (b.z - a.z) * time
			);
		}

		static inline glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float time) {
			return glm::vec2(
				a.x + (b.x - a.x) * time,
				a.y + (b.y - a.y) * time
			);
		}

		static inline float lerp(float a, float b, float time) {
			return a + (b - a) * time;
		}

		static inline std::string& ltrim(std::string& str) {
			size_t start = str.find_first_not_of(" \t\r\n\v\f");
		
			if (std::string::npos != start)
				str = str.substr(start);

			return str;
		}

		static inline std::string& trim(std::string& str) {
			return ltrim(rtrim(str));
		}

		static inline std::string& rtrim(std::string& str) {
			size_t end = str.find_last_not_of(" \t\r\n\v\f");

			if (std::string::npos != end)
				str = str.substr(0, end + 1);

			return str;
		}

		static inline std::string random_string(size_t length) {
			auto randchar = []() -> char {
				const char charset[] =
					"0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";
				const size_t max_index = (sizeof(charset) - 1);

				return charset[rand() % max_index];
			};

			std::string str(length, 0);
			std::generate_n(str.begin(), length, randchar);

			return str;
		}

		static inline std::vector<std::string> splitPath(const std::string& str, const std::set<char> delimiters) {
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

		static inline std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
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

		static inline std::string joinStrings(const std::vector<std::string>& str, const std::string& delimiter = ",") {
			std::ostringstream stream;
			for (auto& i : str)
			{
				if (&i != &str[0])
					stream << delimiter;

				stream << i;
			}
			
			return stream.str();
		}

		static inline int getFileSize(const std::string& filename)  {
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			int size = (int)in.tellg();
			in.close();
			return size;
		};

		static inline std::string bytesToSize(uint64_t size) {
			char* suffix[] = { "B", "KB", "MB", "GB", "TB" };
			char length = sizeof(suffix) / sizeof(suffix[0]);

			int i = 0;
			double dblBytes = double(size);

			if (size > 1024) {
				for (i = 0; (size / 1024) > 0 && i < length - 1; i++, size /= 1024)
					dblBytes = size / 1024.0;
			}

			static char output[200];
			sprintf(output, "%.02lf %s", dblBytes, suffix[i]);

			// Sprint_f sizeof(dblBytes) + space + sizeof(suffix) / sizeof(suffix[0])

			return std::string(output);
		}

		static inline std::string pad(int value, int n = 1) {
			std::string str;

			for (unsigned int i = 0; i < n; ++i)
				str += "0";

			auto v = std::to_string(value);

			return value <= 9 ? str + v : v;
		};

		static inline std::string remove_extension(const std::string& filename) {
			size_t lastdot = filename.find_last_of(".");
			if (lastdot == std::string::npos) return filename;
			return filename.substr(0, lastdot);
		}

		static void gen_random(char* str, const int len) {
			static const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";

			for (int i = 0; i < len; ++i)
				str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

			str[len] = 0;
		}

		static float randomf(float a, float b) {
			float random = ((float)rand()) / (float)RAND_MAX;
			float diff = b - a;
			float r = random * diff;

			return a + r;
		}

		static glm::vec2 randomPointInCircle(const glm::vec2& center, float radius) {
			float r = radius * sqrt(rand() / (float)(RAND_MAX));
			float theta = rand() / (float)(RAND_MAX) * 2 * PI;

			return glm::vec2(
				center.x + r * cos(theta),
				center.y + r * sin(theta)
			);
		}

		static inline float mapValues(float rangeA[], float rangeB[], float value, unsigned int size = 2) {
			return (value - rangeA[0]) * (rangeB[1] - rangeB[0]) / (rangeA[1] - rangeA[0]) + rangeB[0];
		}

		static inline double mapValues(double rangeA[], double rangeB[], double value, unsigned int size = 2) {
			return (value - rangeA[0]) * (rangeB[1] - rangeB[0]) / (rangeA[1] - rangeA[0]) + rangeB[0];
		}

		static char* cpToUTF8(int cp, char* str) {
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
		static void copyMatrix(F& from, T& to) {
			to[0][0] = from.a1; to[1][0] = from.a2;
			to[2][0] = from.a3; to[3][0] = from.a4;
			to[0][1] = from.b1; to[1][1] = from.b2;
			to[2][1] = from.b3; to[3][1] = from.b4;
			to[0][2] = from.c1; to[1][2] = from.c2;
			to[2][2] = from.c3; to[3][2] = from.c4;
			to[0][3] = from.d1; to[1][3] = from.d2;
			to[2][3] = from.d3; to[3][3] = from.d4;
		}

		static std::string fileDialog(int flags = OFN_FILEMUSTEXIST) {
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			wchar_t buffer[MAX_PATH] = L"";

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.lpstrFile = (LPSTR)buffer;
			ofn.nFilterIndex = 1;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_DONTADDTORECENT | flags;

			return GetOpenFileNameA(&ofn) ? std::string(ofn.lpstrFile) : "";
		}

		static std::string folderDialog() {
			return fileDialog(OFN_PATHMUSTEXIST);
		}

		template<class T>
		static std::string numberFormatLocale(T value, char sep = ' ') {
			struct Numpunct : public std::numpunct<char> 
			{
				Numpunct(char separator) : separator(separator) {}

			protected:
				char separator;
				virtual char do_thousands_sep() const { return separator; }
				virtual std::string do_grouping() const { return "\03"; }
			};

			std::stringstream ss;
			ss.imbue({ std::locale(), new Numpunct(sep) });
			ss << std::setprecision(2) << std::fixed << value;

			return ss.str();
		}
	};

}
