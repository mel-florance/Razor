#include "rzpch.h"
#include "AtmosphereMaterial.h"

namespace Razor
{

	AtmosphereMaterial::AtmosphereMaterial() :
		Material(),
		ray_origin(glm::vec3(0.0f, 6371.0f, 0.0f)),
		planet_radius(6371.0f),
		atmosphere_radius(6471.0f),
		rayleigh_scattering(glm::vec3(5.5e-6, 13.0e-6, 22.4e-6)),
		mie_scattering(0.001f),
		rayleigh_scale_height(8000.0f),
		mie_scale_height(1.2e3f),
		mie_scattering_direction(0.968f),
		sun_factor(50.0f),
		limits(glm::vec2(0.0f, 0.01f))
	{
	}

	AtmosphereMaterial::~AtmosphereMaterial()
	{
	}

	void AtmosphereMaterial::bind(Shader* shader)
	{
		if (shader != nullptr)
		{
			shader->setUniform1f("sun_factor", sun_factor);
			shader->setUniform3f("ray_origin", ray_origin);
			shader->setUniform1f("planet_radius", planet_radius);
			shader->setUniform1f("atmosphere_radius", atmosphere_radius);
			shader->setUniform3f("rayleigh_scattering", rayleigh_scattering);
			shader->setUniform1f("mie_scattering", mie_scattering);
			shader->setUniform1f("rayleigh_scale_height", rayleigh_scale_height);
			shader->setUniform1f("mie_scale_height", mie_scale_height);
			shader->setUniform1f("mie_scattering_direction", mie_scattering_direction);
			shader->setUniform2f("limits", limits);
		}
	}

}
