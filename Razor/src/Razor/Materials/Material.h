#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Types/Variant.h"
#include "Razor/Materials/Shader.h"

namespace Razor {

	class RAZOR_API Material
	{
	public:
		Material();
		virtual ~Material();

	private:
		std::shared_ptr<Shader> shader;
		std::map<std::string, Variant> properties;

		unsigned int diffuseMap;
		unsigned int specularMap;
		float shininess;
	};

}
