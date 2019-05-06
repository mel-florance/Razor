#pragma once

namespace Razor
{
	class Particle;

	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void update(float dt);

		typedef std::vector<Particle*> ParticleArray;

		inline ParticleArray& getParticles() { return particles; }

		void addParticle(
			const glm::vec3& position = glm::vec3(0.0f),
			const glm::vec3& velocity = glm::vec3(0.0f),
			float gravity = -9.81f,
			float life = 0.0f,
			float rotation = 0.0f,
			float scale = 1.0f
		);

	private:
		ParticleArray particles;
	};

}
