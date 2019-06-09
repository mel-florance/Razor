#include "rzpch.h"
#include "Texture.h"
#include "Razor/Core/Utils.h"

#include "glad/glad.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor {

	Texture::Texture(const std::string& filename, bool mipmaps, bool flipped, ChannelType type, bool free_after_load) :
		filename(filename),
		mipmaps(mipmaps),
		lodBias(0.0f),
		flipped(flipped),
		id(NULL),
		data(NULL),
		channel_type(type),
		free_after_load(free_after_load)
	{
		glGenTextures(1, &id);
		this->load();
	}

	Texture* Texture::Texture::load()
	{
		stbi_set_flip_vertically_on_load(flipped);
		data = stbi_load(filename.c_str(), &width, &height, &components_count, channel_type);

		if (data == NULL)
		{
			Log::error("Texture loading failed: %s", filename.c_str());
			return nullptr;
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

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

		Log::info("Loaded texture: %s %s", filename.c_str(), Utils::bytesToSize(Utils::getFileSize(filename)).c_str());
	
		if(free_after_load)
			stbi_image_free(data);

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
		if(!free_after_load)
			stbi_image_free(data);

		glDeleteTextures(1, &id);
	}

}
