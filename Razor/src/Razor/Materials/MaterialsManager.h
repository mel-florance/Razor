#pragma once

namespace Razor
{

	class Material;

	class MaterialsManager
	{
	public:
		MaterialsManager();
		~MaterialsManager();

		typedef std::vector<std::shared_ptr<Material>> MaterialList;

		void addMaterial(std::shared_ptr<Material> material);
		bool removeMaterial(const std::string& name);
		std::shared_ptr<Material> getMaterial(const std::string& name);
		inline MaterialList& getMaterials() { return materials; }

	private:
		MaterialList materials;
	};

}
