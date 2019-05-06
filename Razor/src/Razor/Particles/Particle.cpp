#include "rzpch.h"
#include "Particle.h"

namespace Razor
{

	Particle::Particle(
		const glm::vec3& position,
		const glm::vec3& velocity,
		float gravity,
		float life,
		float rotation,
		float scale
	) :
		position(position),
		velocity(velocity),
		gravity(gravity),
		life(life),
		rotation(rotation),
		scale(scale),
		alive(true)
	{
	}

	Particle::~Particle()
	{
	}

	bool Particle::update(float dt)
	{
		velocity.y += gravity * dt;
		position += velocity * dt;
		elapsed += dt;
		alive = elapsed < life;

		return alive;
	}

}
