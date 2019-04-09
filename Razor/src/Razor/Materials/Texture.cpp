#include "rzpch.h"
#include "Texture.h"
#include "Razor/Core/Utils.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor {

	Texture::Texture(const std::string& filename, bool mipmaps) :
		filename(filename),
		mipmaps(mipmaps),
		lodBias(0.0f)
	{
		this->load();
	}

	Texture* Texture::Texture::load()
	{
		int width, height, numComponents;
		unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);

		if (imageData == NULL) {
			RZ_ERROR("Texture loading failed for texture: {0}", filename);
			return nullptr;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			//glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, m_lodBias);
		}
		else
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		RZ_INFO("Loaded texture: {0} : {1}", filename, Utils::getFileSize(filename));

		stbi_image_free(imageData);

		return this;
	}

	void Texture::bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
	}

}
