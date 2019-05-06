#include "rzpch.h"
#include "ColorMaterial.h"

namespace Razor
{

	ColorMaterial::ColorMaterial() : 
		Material(),
		color(glm::vec3(1.0f, 1.0f, 1.0f))
	{
	}

	ColorMaterial::~ColorMaterial()
	{
	}

	void ColorMaterial::bind(Shader * shader)
	{
		if (shader != nullptr)
		{
			shader->setUniform3f("color", color);
		}
	}

}
