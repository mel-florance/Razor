#include "rzpch.h"
#include "ColorMaterial.h"

namespace Razor
{

	ColorMaterial::ColorMaterial(const glm::vec3& color) : 
		Material(),
		color(color)
	{
	}

	ColorMaterial::~ColorMaterial()
	{
	}

	void ColorMaterial::bind(Shader* shader)
	{
		if (shader != nullptr)
		{
			shader->setUniform3f("color", color);
		}
	}

}
