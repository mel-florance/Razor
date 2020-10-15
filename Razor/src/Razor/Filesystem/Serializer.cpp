#include "rzpch.h"
#include "Serializer.h"

namespace Razor
{

	Serializer::Serializer()
	{
	}

	Serializer::~Serializer()
	{
	}

	/*void Serializer::importScene(const std::string& filename)
	{
		Scene scene;
		std::ifstream in(filename.c_str(), std::ios::binary);
		std::string name;
		bool active;
		unsigned int nodes_size;

		readData<std::string>(in, &name);
		readData<unsigned int>(in, &nodes_size);
		readData<bool>(in, &active);

		in.close();

		scene.setName(name);
		scene.setActive(active);

		Log::trace("Imported scene \"%s\". (%d nodes read)", scene.getName().c_str(), nodes_size);
	}

	void Serializer::exportScene(Scene* scene, const std::string& filename)
	{
		std::ofstream out("./data/test.rza", std::ios::binary);

		unsigned int nodes_size = (unsigned int)scene->getNodesSize();

		writeData<std::string>(out, &scene->getName());
		writeData<unsigned int>(out, &nodes_size);
		writeData<bool>(out, &scene->isActive());

		out.close();

		Log::trace("Exported scene \"%s\". (%d nodes written)", scene->getName().c_str(), nodes_size);
	}*/

}
