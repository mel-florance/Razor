#pragma once

namespace Razor
{

	class Particle
	{
	public:
		Particle(
			const glm::vec3& position = glm::vec3(0.0f),
			const glm::vec3& velocity = glm::vec3(0.0f),
			float gravity = -9.81f,
			float life = 0.0f,
			float rotation = 0.0f,
			float scale = 1.0f
		);
		~Particle();

		bool update(float dt);

		inline glm::vec3& getPosition() { return position; }
		inline void const setPosition(const glm::vec3& pos) { position = pos; }

		inline glm::vec3& getVelocity() { return velocity; }
		inline void const setVelocity(const glm::vec3& vel) { velocity = vel; }

		inline float getGravity() const { return gravity; }
		inline void const setGravity(float value) { gravity = value; }

		inline float getLife() const { return life; }
		inline void const setLife(float value) { life = value; }

		inline float getRotation() const { return rotation; }
		inline void const setRotation(float value) { rotation = value; }

		inline float getScale() const { return scale; }
		inline void const setScale(float value) { scale = value; }

		inline float getElapsed() const { return elapsed; }
		inline void const setElapsed(float value) { elapsed = value; }

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		float gravity;
		float life;
		float rotation;
		float scale;
		float elapsed;
		bool alive;
	};

}
