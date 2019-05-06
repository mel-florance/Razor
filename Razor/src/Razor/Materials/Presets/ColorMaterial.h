#pragma once

#include "Razor/Materials/Material.h"

namespace Razor
{

	class ColorMaterial : public Material
	{
	public:
		ColorMaterial();
		~ColorMaterial();

		void bind(Shader* shader) override;

		inline glm::vec3& getColor() { return color; }
		inline void setColor(const glm::vec3& color) { this->color = color; }

	private:
		glm::vec3 color;
	};

}
