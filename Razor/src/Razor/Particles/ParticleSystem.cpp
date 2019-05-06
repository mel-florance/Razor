#include "rzpch.h"
#include "ParticleSystem.h"
#include "Particle.h"

namespace Razor
{

	ParticleSystem::ParticleSystem()
	{
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::update(float dt)
	{
		ParticleArray::iterator it;

		for (it = particles.begin(); it != particles.end(); ++it)
		{
			if (!(*it)->update(dt))
				particles.erase(it);
		}
	}

	void ParticleSystem::addParticle(
		const glm::vec3& position,
		const glm::vec3& velocity,
		float gravity,
		float life,
		float rotation,
		float scale
	)
	{
		Particle* particle = new Particle(
			position,
			velocity,
			gravity,
			life,
			rotation,
			scale
		);

		particles.push_back(particle);
	}

}
