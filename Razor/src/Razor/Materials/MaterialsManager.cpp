#include "rzpch.h"
#include "MaterialsManager.h"
#include "Material.h"

namespace Razor
{

	MaterialsManager::MaterialsManager()
	{

	}

	MaterialsManager::~MaterialsManager()
	{

	}

	void MaterialsManager::addMaterial(std::shared_ptr<Material> material)
	{
		materials.push_back(material);
	}

	bool MaterialsManager::removeMaterial(const std::string& name)
	{
		auto it = std::find_if(materials.begin(), materials.end(),
		[&](const std::shared_ptr<Material>& mat) {
			return mat->getName() == name;
		});

		if (it != materials.end())
		{
			materials.erase(it);
			(*it).reset();

			return true;
		}

		return false;
	}

	std::shared_ptr<Material> MaterialsManager::getMaterial(const std::string& name)
	{
		auto it = std::find_if(materials.begin(), materials.end(),
		[&](const std::shared_ptr<Material>& mat) {
			return mat->getName() == name;
		});

		if (it != materials.end())
			return *it;

		return nullptr;
	}

}
