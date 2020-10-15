#include "rzpch.h"
#include <glad/glad.h>
#include "EnvironmentTexture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Razor
{

	EnvironmentTexture::EnvironmentTexture(const std::string& filename, bool flipped) :
		filename(filename),
		flipped(flipped)
	{
		load();
	}

	EnvironmentTexture::~EnvironmentTexture()
	{

	}

	void EnvironmentTexture::load()
	{
		stbi_set_flip_vertically_on_load(flipped);
		float* data = stbi_loadf(filename.c_str(), &width, &height, &nb_components, 0);

		if (data)
		{
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);

			std::string size = std::string("(" + std::to_string(width) + "x" + std::to_string(height) + ")");
			Log::info("Loaded environment texture: %s %s %s", filename.c_str(), size.c_str(), Utils::bytesToSize(Utils::getFileSize(filename)).c_str());
		}
		else
			std::cout << "Failed to load environment texture: " << filename << std::endl;
	}

}