#include "rzpch.h"
#include "Texture.h"
#include "Razor/Core/Utils.h"

#include "glad/glad.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor {

	Texture::Texture(const std::string& filename, bool mipmaps, bool flipped) :
		filename(filename),
		mipmaps(mipmaps),
		lodBias(0.0f),
		flipped(flipped),
		id(NULL)
	{
		this->load();
	}

	Texture* Texture::Texture::load()
	{
		int width, height, numComponents;
		stbi_set_flip_vertically_on_load(flipped);

		unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);

		if (imageData == NULL) {
			Log::error("Texture loading failed for texture: {0}", filename);
			return nullptr;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipmaps) 
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, -1.0f);
		}
		else
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		float anisotropy = 0.6f;
		glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY, &anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, anisotropy);

		Log::info("Loaded texture: %s : %s", filename.c_str(), Utils::getFileSize(filename).c_str());

		stbi_image_free(imageData);

		return this;
	}

	void Texture::bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

}
