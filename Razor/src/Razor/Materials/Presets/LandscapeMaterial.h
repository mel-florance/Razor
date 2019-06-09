#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class LandscapeMaterial : public Material
	{
	public:
		LandscapeMaterial();
		~LandscapeMaterial();

		void bind(Shader* shader) override;

		inline void setDiffuseColor(const glm::vec3& color) { diffuse_color = color; }
		inline void setSpecularColor(const glm::vec3& color) { specular_color = color; }
		inline void setAmbientColor(const glm::vec3& color) { ambient_color = color; }
		inline void setEmissiveColor(const glm::vec3& color) { emissive_color = color; }
		inline void setShininess(float value) { shininess = value; }
		inline void setShininessStrength(float value) { shininess_strength = value; }
		inline void setAlpha(float value) { alpha = value; }
		inline void setNormalStrength(float value) { normal_strength = value; }
		inline void setDiffuseTiling(const glm::vec2& tiling) { diffuse_tiling = tiling; }
		inline void setSpecularTiling(const glm::vec2& tiling) { specular_tiling = tiling; }
		inline void setNormalTiling(const glm::vec2& tiling) { normal_tiling = tiling; }

		inline void setRedChannelDiffuse(unsigned int id) { red_channel_diffuse = id; has_red_channel_diffuse = true; }
		inline void setRedChannelSpecular(unsigned int id) { red_channel_specular = id; has_red_channel_specular = true; }
		inline void setRedChannelNormal(unsigned int id) { red_channel_normal = id; has_red_channel_normal = true; }
		inline void setGreenChannelDiffuse(unsigned int id) { green_channel_diffuse = id; has_green_channel_diffuse = true;}
		inline void setGreenChannelSpecular(unsigned int id) { green_channel_specular = id; has_green_channel_specular = true;}
		inline void setGreenChannelNormal(unsigned int id) { green_channel_normal = id; has_green_channel_normal = true;}
		inline void setBlueChannelDiffuse(unsigned int id) { blue_channel_diffuse = id; has_blue_channel_diffuse = true; }
		inline void setBlueChannelSpecular(unsigned int id) { blue_channel_specular = id; has_blue_channel_specular = true; }
		inline void setBlueChannelNormal(unsigned int id) { blue_channel_normal = id; has_blue_channel_normal = true; }
		inline void setSplatmap(unsigned int id) { splatmap = id; has_splatmap = true; }

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

		glm::vec2 diffuse_tiling;
		glm::vec2 specular_tiling;
		glm::vec2 normal_tiling;

		float alpha;
		float shininess;
		float shininess_strength;
		float normal_strength;

		bool has_red_channel_diffuse;
		bool has_red_channel_specular;
		bool has_red_channel_normal;
		bool has_green_channel_diffuse;
		bool has_green_channel_specular;
		bool has_green_channel_normal;
		bool has_blue_channel_diffuse;
		bool has_blue_channel_specular;
		bool has_blue_channel_normal;
		bool has_splatmap;

		unsigned int red_channel_diffuse;
		unsigned int red_channel_specular;
		unsigned int red_channel_normal;
		unsigned int green_channel_diffuse;
		unsigned int green_channel_specular;
		unsigned int green_channel_normal;
		unsigned int blue_channel_diffuse;
		unsigned int blue_channel_specular;
		unsigned int blue_channel_normal;
		unsigned int splatmap;

		std::string red_channel_diffuse_path;
		std::string red_channel_specular_path;
		std::string red_channel_normal_path;
		std::string green_channel_diffuse_path;
		std::string green_channel_specular_path;
		std::string green_channel_normal_path;
		std::string blue_channel_diffuse_path;
		std::string blue_channel_specular_path;
		std::string blue_channel_normal_path;

		std::string splatmap_path;
	};

}

