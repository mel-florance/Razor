#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class PhongMaterial : public Material
	{
	public:
		PhongMaterial();
		~PhongMaterial();

		void bind(Shader* shader) override;

		inline void setDiffuseColor(const glm::vec3& color) { diffuse_color = color; }
		inline glm::vec3 getDiffuseColor() { return diffuse_color; }

		inline void setSpecularColor(const glm::vec3& color) { specular_color = color; }
		inline glm::vec3 getSpecularColor() { return specular_color; }

		inline void setAmbientColor(const glm::vec3& color) { ambient_color = color; }
		inline glm::vec3 getAmbientColor() { return ambient_color; }

		inline void setEmissiveColor(const glm::vec3& color) { emissive_color = color; }
		inline glm::vec3 getEmissiveColor() { return emissive_color; }

		inline void setShininess(float value) { shininess = value; }
		inline float getShininess() { return shininess; }

		inline void setShininessStrength(float value) { shininess_strength = value; }
		inline float getShininessStrength() { return shininess_strength; }

		inline void setAlpha(float value) { alpha = value; }
		inline float getAlpha() { return alpha; }

	private:
		glm::vec3 diffuse_color;
		glm::vec3 specular_color;
		glm::vec3 ambient_color;
		glm::vec3 emissive_color;

		unsigned int diffuse_map;
		unsigned int specular_map;
		unsigned int normal_map;

		float alpha;
		float shininess;
		float shininess_strength;
		float normal_strength;
	};

}

