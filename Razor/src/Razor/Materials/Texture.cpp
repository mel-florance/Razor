#include "rzpch.h"
#include "Texture.h"
#include "Razor/Core/Utils.h"

#include "glad/glad.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor
{

	Texture::Texture(
		const std::string& filename,
		bool mipmaps,
		bool flipped,
		ChannelType type,
		bool free_after_load
	) :
		filename(filename),
		mipmaps(mipmaps),
		lodBias(0.0f),
		flipped(flipped),
		id(NULL),
		data(NULL),
		channel_type(type),
		free_after_load(free_after_load),
		min_filter(Filter::LINEAR),
		mag_filter(Filter::LINEAR),
		wrap_s(WrapType::REPEAT),
		wrap_t(WrapType::REPEAT)

	{
		glGenTextures(1, &id);
		this->load();
	}

	Texture* Texture::Texture::load()
	{
		stbi_set_flip_vertically_on_load(flipped);
		data = stbi_load(filename.c_str(), &width, &height, &components_count, 0);

		if (data == NULL)
		{
			Log::error("Texture loading failed: %s", filename.c_str());
			return nullptr;
		}

		glBindTexture(GL_TEXTURE_2D, id);

		GLenum format;
		if (components_count == 1)
			format = GL_RED;
		else if (components_count == 3)
			format = GL_RGB;
		else if (components_count == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::string size = std::string("(" + std::to_string(width) + "x" + std::to_string(height) + ")");
		Log::info("Loaded texture: %s %s %s", filename.c_str(), size.c_str(), Utils::bytesToSize(Utils::getFileSize(filename)).c_str());
	
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
