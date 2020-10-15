#pragma once

#include "Razor/Scene/Scene.h"

namespace Razor
{
	class Engine;

	class Serializer
	{
	public:
		Serializer();
		~Serializer();

		//void importScene(const std::string& filename);
		//void exportScene(Scene* scene, const std::string& filename);

		template<typename T>
		void readData(std::ifstream& stream, T* data)
		{
			stream.read(reinterpret_cast<char*>(data), sizeof(T));
		}

		template<typename T>
		void writeData(std::ofstream& stream, T* data)
		{
			stream.write(reinterpret_cast<char*>(data), sizeof(T));
		}

		//template<typename T>
		//inline void readArray(std::ifstream& stream, std::vector<T>* array)
		//{

		//}
	};

}
