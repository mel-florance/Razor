#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

namespace Razor
{

	class PhysicsBody;

	class World
	{
	public:
		World();
		~World();

		void tick(float delta);

		inline glm::vec3& getGravity() { return gravity; }
		void setGravity(const glm::vec3& gravity);

		void addBody(PhysicsBody* body);
		inline std::vector<PhysicsBody*>& getBodies() { return bodies; }

	private:
		btDynamicsWorld* world;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btSequentialImpulseConstraintSolver* solver;
		btCollisionConfiguration* config;

		glm::vec3 gravity;

		std::vector<PhysicsBody*> bodies;
	};

}
