#pragma once

#include "Texture.h"

namespace Razor
{

	class TextureAtlas : public Texture
	{
	public:
		TextureAtlas(const std::string& filename, bool mipmaps = false, bool flipped = true);
		~TextureAtlas();

		inline void addItem(const std::string& name, unsigned int x, unsigned int y)
		{
			items_coords[name] = glm::uvec2(x, y);
			items_positions[name] = calculateGridPos(x, y);
		}

		inline const glm::uvec2 getItemCoords(const std::string& name) const
		{
			auto item = items_coords.find(name);

			if (item != items_coords.end())
				return item->second;

			return glm::uvec2();
		}

		inline const glm::vec4 getItemPosition(const std::string& name) const
		{
			auto item = items_positions.find(name);

			if (item != items_positions.end())
				return item->second;

			return glm::vec4();
		}

	private:
		inline const glm::vec4 calculateGridPos(unsigned int x, unsigned int y)
		{
			int w = getWidth(), h = getHeight();

			return glm::vec4(
				(min.x + size.x * y) / w,
				(min.y + size.y * x) / h,
				(max.x + size.x * y) / w,
				(max.y + size.y * x) / h
			);
		}

		glm::vec2 min;
		glm::vec2 max;
		glm::vec2 size;

		std::unordered_map<std::string, glm::uvec2> items_coords;
		std::unordered_map<std::string, glm::vec4> items_positions;
	};

}
