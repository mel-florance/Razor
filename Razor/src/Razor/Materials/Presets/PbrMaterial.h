#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class PbrMaterial : public Material
	{
	public:
		PbrMaterial();
		~PbrMaterial();
		
		void bind(Shader* shader);
	private:
	};

}
