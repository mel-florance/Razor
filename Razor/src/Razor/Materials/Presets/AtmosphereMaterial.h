#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class AtmosphereMaterial : public Material
	{
	public:
		AtmosphereMaterial();
		~AtmosphereMaterial();

		void bind(Shader* shader) override;

		inline glm::vec3& getRayOrigin() { return ray_origin; }
		inline float& getPlanetRadius() { return planet_radius; }
		inline float& getAtmosphereradius() { return atmosphere_radius; }
		inline glm::vec3& getRayleighScattering() { return rayleigh_scattering; }
		inline float& getMieScattering() { return mie_scattering; }
		inline float& getRayleighScaleHeight() { return rayleigh_scale_height; }
		inline float& getMieScaleHeight() { return mie_scale_height; }
		inline float& getMieScatteringDirection() { return mie_scattering_direction; }
		inline float& getSunFactor() { return sun_factor; }
		inline glm::vec2& getLimits() { return limits; }

	private:
		glm::vec3 ray_origin;
		float planet_radius;
		float atmosphere_radius;
		glm::vec3 rayleigh_scattering;
		float mie_scattering;
		float rayleigh_scale_height;
		float mie_scale_height;
		float mie_scattering_direction;
		float sun_factor;
		glm::vec2 limits;
	};

}
