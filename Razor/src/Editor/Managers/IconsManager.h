#pragma once

namespace Razor
{
	class TextureAtlas;

	class IconsManager
	{
	public:
		IconsManager();
		~IconsManager();

		enum class IconType
		{
			IMAGE,
			BUTTON
		};

		void drawIcon(const std::string& name, const glm::vec2& size, IconType type = IconType::IMAGE);

	private:
		TextureAtlas* atlas;
	};

}