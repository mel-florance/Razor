#include "rzpch.h"
#include "SpherePhysicsBody.h"

namespace Razor
{

	SpherePhysicsBody::SpherePhysicsBody(float radius, const glm::vec3& position) :
		PhysicsBody(position),
		radius(radius)
	{
		shape = new btSphereShape(radius);
	}

	SpherePhysicsBody::~SpherePhysicsBody()
	{
	}

	void SpherePhysicsBody::init()
	{
		btVector3 inertia(0, 0, 0);
		((btCollisionShape*)shape)->calculateLocalInertia(mass, inertia);
		body = new btRigidBody({ mass, motion_state, (btSphereShape*)shape, inertia });

		body->setRestitution(0.5f);
		body->setFriction(0.5f);
		body->setRollingFriction(0.5f);
		body->setActivationState(DISABLE_DEACTIVATION);

		initialized = true;
	}

}
