#include "rzpch.h"
#include "PbrMaterial.h"
#include "glad/glad.h"

namespace Razor
{

	PbrMaterial::PbrMaterial() : Material()
	{
	}

	PbrMaterial::~PbrMaterial()
	{
	}

	void PbrMaterial::bind(Shader* shader)
	{
		if (shader != nullptr)
		{
			if (hasDiffuseMap())
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, getDiffuseMap());
			}

			if (hasNormalMap())
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, getNormalMap());
			}

			if (hasMetallicMap())
			{
				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, getMetallicMap());
			}

			if (hasRoughnessMap())
			{
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_2D, getRoughnessMap());
			}

			if (hasAoMap())
			{
				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_2D, getAoMap());
			}

			if (hasOrmMap())
			{
				glActiveTexture(GL_TEXTURE8);
				glBindTexture(GL_TEXTURE_2D, getOrmMap());
			}

			if (hasOpacityMap())
			{
	
				glActiveTexture(GL_TEXTURE9);
				glBindTexture(GL_TEXTURE_2D, getOpacityMap());
				glDisable(GL_BLEND);
			}

			if (hasEmissiveMap())
			{
				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, getEmissiveMap());
			}

			shader->setUniform1i("hasAlbedo", (int)hasDiffuseMap());
			shader->setUniform1i("hasNormal", (int)hasNormalMap());
			shader->setUniform1i("hasMetallic", (int)hasMetallicMap());
			shader->setUniform1i("hasRoughness", (int)hasRoughnessMap());
			shader->setUniform1i("hasAo", (int)hasAoMap());
			shader->setUniform1i("hasOrm", (int)hasOrmMap());
			shader->setUniform1i("hasOpacity", (int)hasOpacityMap());
			shader->setUniform1i("hasEmissive", (int)hasEmissiveMap());
		}
	}

}
