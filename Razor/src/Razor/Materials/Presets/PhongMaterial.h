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
		inline void setSpecularColor(const glm::vec3& color) { specular_color = color; }
		inline void setAmbientColor(const glm::vec3& color) { ambient_color = color; }
		inline void setEmissiveColor(const glm::vec3& color) { emissive_color = color; }
		inline void setShininess(float value) { shininess = value; }
		inline void setShininessStrength(float value) { shininess_strength = value; }
		inline void setAlpha(float value) { alpha = value; }
		inline void setNormalStrenght(float value) { normal_strength = value; }
		inline void setDiffuseTiling(const glm::vec2& tiling) { diffuse_tiling = tiling; }
		inline void setSpecularTiling(const glm::vec2& tiling) { specular_tiling = tiling; }
		inline void setNormalTiling(const glm::vec2& tiling) { normal_tiling = tiling; }


		inline glm::vec3& getDiffuseColor() { return diffuse_color; }
		inline glm::vec3& getSpecularColor() { return specular_color; }
		inline glm::vec3& getAmbientColor() { return ambient_color; }
		inline glm::vec3& getEmissiveColor() { return emissive_color; }

		inline float& getShininess() { return shininess; }
		inline float& getShininessStrength() { return shininess_strength; }
		inline float& getAlpha() { return alpha; }
		inline float& getNormalStrength() { return normal_strength; }

		inline glm::vec2& getDiffuseTiling() { return diffuse_tiling; }
		inline glm::vec2& getSpecularTiling() { return specular_tiling; }
		inline glm::vec2& getNormalTiling() { return normal_tiling; }

	private:
		glm::vec3 diffuse_color;
		glm::vec3 specular_color;
		glm::vec3 ambient_color;
		glm::vec3 emissive_color;

		unsigned int diffuse_map;
		unsigned int specular_map;
		unsigned int normal_map;

		glm::vec2 diffuse_tiling;
		glm::vec2 specular_tiling;
		glm::vec2 normal_tiling;

		float alpha;
		float shininess;
		float shininess_strength;
		float normal_strength;

	};

}

