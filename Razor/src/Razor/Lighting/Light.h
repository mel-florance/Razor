#pragma once

#include <glm/glm.hpp>
#include "ShadowGenerator.h"
#include "LightBound.h"

namespace Razor 
{

	class Node;
	class Camera;

	class Light
	{
	public:
		Light(
			Camera* camera,
			const glm::vec3& ambient  = glm::vec3(0.1f, 0.1f, 0.1f),
			const glm::vec3& diffuse  = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		virtual ~Light();

		enum class Type
		{
			DIRECTIONAL,
			POINT,
			SPOT,
			AREA
		};

		inline glm::vec3& getAmbient() { return ambient; }
		inline glm::vec3& getDiffuse() { return diffuse; }
		inline glm::vec3& getSpecular() { return specular; }
		inline Type getType() { return type; }
		inline float& getIntensity() { return intensity; }
		inline bool& isCastingShadows() { return cast_shadows; }
		inline ShadowGenerator* getShadowGenerator() { return shadow_generator; }
		inline LightBound* getLightBound() { return light_bound; }

		inline void setAmbient(const glm::vec3& color) { this->ambient = color; }
		inline void setDiffuse(const glm::vec3& color) { this->diffuse = color; }
		inline void setSpecular(const glm::vec3& color) { this->specular = color; }
		inline void setType(Type type) { this->type = type; }
		inline void setIntensity(float value) { intensity = value; }
		inline void setCastingShadows(bool value) { cast_shadows = value; }
		inline void setShadowGenerator(ShadowGenerator* generator) { shadow_generator = generator; }
		inline void setLightBound(LightBound* bound) { light_bound = bound; }

	protected:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool cast_shadows;
		Type type;
		float intensity;
		ShadowGenerator* shadow_generator;
		LightBound* light_bound;
		Camera* camera;
	};

}
