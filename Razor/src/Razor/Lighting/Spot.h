#pragma once

#include "Light.h"

namespace Razor {

	class Spot : public Light
	{
	public:
		Spot();
		~Spot();

	private:
		glm::vec3 position;
		glm::vec3 direction;
		float inner_cutoff;
		float outer_cutoff;
		float constant;
		float linear;
		float quadraticc;
	};

}
