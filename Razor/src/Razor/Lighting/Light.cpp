#include "rzpch.h"
#include "Light.h"

namespace Razor {

	Light::Light(
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular
	) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular)
	{
	}


	Light::~Light()
	{
	}

}
