#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class SkyboxMaterial : public Material
	{
	public:
		SkyboxMaterial();
		~SkyboxMaterial();

		void bind(Shader* shader) override;

	private:

	};

}
