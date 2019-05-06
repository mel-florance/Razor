#include "rzpch.h"
#include "PhongMaterial.h"
#include "glad/glad.h"

namespace Razor
{

	PhongMaterial::PhongMaterial() : 
		Material(),
		diffuse_color(glm::vec3(0.8f, 0.8f, 0.8f)),
		specular_color(glm::vec3(1.0f, 1.0f, 1.0f)),
		ambient_color(glm::vec3(0.1f, 0.1f, 0.1f)),
		emissive_color(glm::vec3(1.0f, 1.0f, 1.0f)),
		diffuse_map(0),
		specular_map(1),
		normal_map(2),
		shininess(32.0f),
		shininess_strength(1.0f),
		alpha(1.0f),
		normal_strength(1.0f)
	{
	}

	PhongMaterial::~PhongMaterial()
	{
	}

	void PhongMaterial::bind(Shader* shader)
	{
		if (shader != nullptr)
		{
			if (hasDiffuseMap())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, getDiffuseMap());
			}

			if (hasSpecularMap())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, getSpecularMap());
			}

			if (hasNormalMap())
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, getNormalMap());
			}

			shader->setUniform1i("material.diffuseMap", 0);
			shader->setUniform1i("material.specularMap", 1);
			shader->setUniform1i("material.normalMap", 2);
			shader->setUniform1i("material.shadowMap", 3);

			shader->setUniform3f("material.diffuseColor", diffuse_color);
			shader->setUniform3f("material.specularColor", specular_color);
			shader->setUniform3f("material.ambientColor", ambient_color);

			shader->setUniform1f("material.shininess", shininess);
			shader->setUniform1f("material.shininess_strength", shininess_strength);
			shader->setUniform1f("material.normal_strength", normal_strength);

			shader->setUniform1i("material.hasDiffuse", (int)hasDiffuseMap());
			shader->setUniform1i("material.hasSpecular", (int)hasSpecularMap());
			shader->setUniform1i("material.hasNormal", (int)hasNormalMap());
		}
	}

}
