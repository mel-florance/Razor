#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class RAZOR_API Material
	{
	public:
		Material();
		~Material();

	private:
		unsigned int diffuseMap;
		unsigned int specularMap;
		float shininess;
	};

}
