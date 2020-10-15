#include "rzpch.h"
#include "Asset.h"

namespace Razor
{

	Asset::Asset(const std::string& path) :
		Serializer(),
		path(path)
	{
	}

	Asset::~Asset()
	{
	}

}
