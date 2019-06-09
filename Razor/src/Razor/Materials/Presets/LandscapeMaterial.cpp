#include "rzpch.h"
#include "LandscapeMaterial.h"
#include "glad/glad.h"

namespace Razor
{

	LandscapeMaterial::LandscapeMaterial() :
		Material(),
		diffuse_color(glm::vec3(1.0f, 1.0f, 1.0f)),
		specular_color(glm::vec3(0.8f, 0.8f, 0.8f)),
		ambient_color(glm::vec3(1.0f, 1.0f, 1.0f)),
		emissive_color(glm::vec3(1.0f, 1.0f, 1.0f)),

		diffuse_tiling(glm::vec2(1.0f, 1.0f)),
		specular_tiling(glm::vec2(1.0f, 1.0f)),
		normal_tiling(glm::vec2(1.0f, 1.0f)),

		shininess(32.0f),
		shininess_strength(1.0f),

		alpha(1.0f),
		normal_strength(1.0f),

		has_red_channel_diffuse(false),
		has_green_channel_diffuse(false),
		has_blue_channel_diffuse(false),
		has_splatmap(false)
	{
	}

	LandscapeMaterial::~LandscapeMaterial()
	{
	}

	void LandscapeMaterial::bind(Shader* shader)
	{
		if (shader != nullptr)
		{
			if (hasDiffuseMap())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, getDiffuseMap());
				shader->setUniform1i("material.diffuseMap", 0);
			}

			if (hasSpecularMap())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, getSpecularMap());
				shader->setUniform1i("material.specularMap", 1);
			}

			if (hasNormalMap())
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, getNormalMap());
				shader->setUniform1i("material.normalMap", 2);
			}

			shader->setUniform1i("material.shadowMap", 3);

			if (has_splatmap)
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, splatmap);
				shader->setUniform1i("material.splatmap", 4);
			}

			if (has_red_channel_diffuse)
			{
				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, red_channel_diffuse);
				shader->setUniform1i("material.red_channel_diffuse", 5);
			}

			if (has_red_channel_specular)
			{
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_2D, red_channel_specular);
				shader->setUniform1i("material.red_channel_specular", 6);
			}

			if (has_red_channel_normal)
			{
				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_2D, red_channel_normal);
				shader->setUniform1i("material.red_channel_normal", 7);
			}

			if (has_green_channel_diffuse)
			{
				glActiveTexture(GL_TEXTURE8);
				glBindTexture(GL_TEXTURE_2D, green_channel_diffuse);
				shader->setUniform1i("material.green_channel_diffuse", 8);
			}

			if (has_green_channel_specular)
			{
				glActiveTexture(GL_TEXTURE9);
				glBindTexture(GL_TEXTURE_2D, green_channel_specular);
				shader->setUniform1i("material.green_channel_specular", 9);
			}

			if (has_green_channel_normal)
			{
				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, green_channel_normal);
				shader->setUniform1i("material.green_channel_normal", 10);
			}

			if (has_blue_channel_diffuse)
			{
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, blue_channel_diffuse);
				shader->setUniform1i("material.blue_channel_diffuse", 11);
			}

			if (has_blue_channel_specular)
			{
				glActiveTexture(GL_TEXTURE12);
				glBindTexture(GL_TEXTURE_2D, blue_channel_specular);
				shader->setUniform1i("material.blue_channel_specular", 12);
			}

			if (has_blue_channel_normal)
			{
				glActiveTexture(GL_TEXTURE13);
				glBindTexture(GL_TEXTURE_2D, blue_channel_normal);
				shader->setUniform1i("material.blue_channel_normal", 13);
			}

			shader->setUniform3f("material.diffuseColor", diffuse_color.x, diffuse_color.y, diffuse_color.z);
			shader->setUniform3f("material.specularColor", specular_color.x, specular_color.y, specular_color.z);
			shader->setUniform3f("material.ambientColor", ambient_color.x, ambient_color.y, ambient_color.z);

			shader->setUniform1f("material.shininess", shininess);
			shader->setUniform1f("material.shininess_strength", shininess_strength);
			shader->setUniform1f("material.normal_strength", normal_strength);

			shader->setUniform1i("material.hasDiffuse", (int)hasDiffuseMap());
			shader->setUniform1i("material.hasSpecular", (int)hasSpecularMap());
			shader->setUniform1i("material.hasNormal", (int)hasNormalMap());

			shader->setUniform1i("material.hasRedChannelDiffuse", (int)has_red_channel_diffuse);
			shader->setUniform1i("material.hasRedChannelSpecular", (int)has_red_channel_specular);
			shader->setUniform1i("material.hasRedChannelNormal", (int)has_red_channel_normal);
			shader->setUniform1i("material.hasGreenChannelDiffuse", (int)has_green_channel_diffuse);
			shader->setUniform1i("material.hasGreenChannelSpecular", (int)has_green_channel_specular);
			shader->setUniform1i("material.hasGreenChannelNormal", (int)has_green_channel_normal);
			shader->setUniform1i("material.hasBlueChannelDiffuse", (int)has_blue_channel_diffuse);
			shader->setUniform1i("material.hasBlueChannelSpecular", (int)has_blue_channel_specular);
			shader->setUniform1i("material.hasBlueChannelNormal", (int)has_blue_channel_normal);
			shader->setUniform1i("material.hasSplatmap", (int)has_splatmap);

			shader->setUniform2f("material.diffuse_tiling", diffuse_tiling);
			shader->setUniform2f("material.specular_tiling", specular_tiling);
			shader->setUniform2f("material.normal_tiling", normal_tiling);
		}
	}

}
