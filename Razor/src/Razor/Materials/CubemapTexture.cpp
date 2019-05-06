#include "rzpch.h"
#include "CubemapTexture.h"

#include "glad/glad.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor
{

	CubemapTexture::CubemapTexture(const std::vector<std::string>& textures) :
		textures(textures)
	{
		this->load();
	}

	CubemapTexture::~CubemapTexture()
	{
	}

	CubemapTexture* CubemapTexture::load()
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			int width, height, numComponents;
			unsigned char* imageData = stbi_load(textures[i].c_str(), &width, &height, &numComponents, 4);

			if (imageData == NULL) {
				std::cerr << "Cube map texture loading failed for: " << textures[i].c_str() << std::endl;
				return 0;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
		}

		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Log::info("Loaded texture: %s", textures[0].c_str());
		glDisable(GL_TEXTURE_CUBE_MAP);

		return this;
	}

	void CubemapTexture::bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void CubemapTexture::unbind()
	{
		glDeleteTextures(1, &id);
	}

}
