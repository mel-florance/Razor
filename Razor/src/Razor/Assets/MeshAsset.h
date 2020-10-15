#pragma once

#include "Razor/Assets/Asset.h"

namespace Razor
{
	class StaticMesh;

	class MeshAsset : public Asset
	{
	public:
		MeshAsset(const std::string& path);
		~MeshAsset() override;

		void load() override;
		void serialize() override;
		void unserialize() override;

	private:
		std::shared_ptr<StaticMesh> data;
	};

}
