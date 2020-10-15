#include "rzpch.h"
#include "MeshAsset.h"
#include "Razor/Geometry/StaticMesh.h"

namespace Razor
{

	MeshAsset::MeshAsset(const std::string& path) :
		Asset(path),
		data(nullptr)
	{
	}

	MeshAsset::~MeshAsset()
	{
	}

	void MeshAsset::load()
	{
	
	}

	void MeshAsset::serialize()
	{
	}

	void MeshAsset::unserialize()
	{
	}

}
