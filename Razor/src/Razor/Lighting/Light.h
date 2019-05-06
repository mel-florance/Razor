#pragma once

#include <glm/glm.hpp>
#include "ShadowGenerator.h"

namespace Razor 
{

	class Light
	{
	public:
		Light(
			const glm::vec3& ambient = glm::vec3(0.15f, 0.15f, 0.15f),
			const glm::vec3& diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		~Light();

		enum class Type
		{
			DIRECTIONAL,
			POINT,
			SPOT,
			AREA
		};

		inline glm::vec3& getAmbient() { return ambient; }
		inline void setAmbient(const glm::vec3& color) { this->ambient = color; }

		inline glm::vec3& getDiffuse() { return diffuse; }
		inline void setDiffuse(const glm::vec3& color) { this->diffuse = color; }

		inline glm::vec3& getSpecular() { return specular; }
		inline void setSpecular(const glm::vec3& color) { this->specular = color; }

		inline void setType(Type type) { this->type = type; }
		inline Type& getType() { return type; }

	private:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool cast_shadows;
		ShadowGenerator shadow_generator;

	protected:
		Type type;
	};

}
