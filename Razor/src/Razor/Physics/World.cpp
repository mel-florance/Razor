#include "rzpch.h"
#include "World.h"
#include "PhysicsBody.h"

namespace Razor
{

	World::World() :
		gravity(glm::vec3(0.0f, -9.81f, 0.0f))
	{
		config = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(config);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();

		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
		world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	World::~World()
	{
		delete world;
		delete dispatcher;
		delete broadphase;
		delete solver;
		delete config;
	}

	void World::tick(float delta)
	{
		world->stepSimulation(delta);
	}

	void World::setGravity(const glm::vec3& gravity)
	{
		this->gravity = gravity;
		world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	void World::addBody(PhysicsBody* body)
	{
		body->init();
		bodies.push_back(body);
		world->addRigidBody(body->getBody());
	}

}
