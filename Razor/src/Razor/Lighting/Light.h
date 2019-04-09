#pragma once

#include <glm/glm.hpp>

namespace Razor {

	class Light
	{
	public:
		Light(
			const glm::vec3& ambient = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		~Light();

		enum class Type {
			DIRECTIONAL,
			POINT,
			SPOT,
			AREA
		};

		inline glm::vec3& getAmbient() { return ambient; }
		inline glm::vec3& getDiffuse() { return diffuse; }
		inline glm::vec3& getSpecular() { return specular; }
		inline Type getType() { return type; }

		inline void setAmbient(const glm::vec3& color) { this->ambient = color; }
		inline void setDiffuse(const glm::vec3& color) { this->diffuse = color; }
		inline void setSpecular(const glm::vec3& color) { this->specular = color; }
		inline void setType(Type type) { this->type = type; }

	private:
		Type type;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

}

