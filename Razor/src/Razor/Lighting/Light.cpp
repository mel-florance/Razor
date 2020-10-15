#include "rzpch.h"
#include "Light.h"

#include "Razor/Rendering/ForwardRenderer.h"
#include "Razor/Cameras/Camera.h"

namespace Razor 
{

	Light::Light(Camera* camera, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :
		camera(camera),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shadow_generator(nullptr),
		light_bound(nullptr),
		cast_shadows(true),
		type(Type::POINT),
		intensity(1.0f)
	{
		//shadow_generator = new ShadowGenerator(camera);
	}

	Light::~Light()
	{
		delete shadow_generator;
		delete light_bound;
	}

}
