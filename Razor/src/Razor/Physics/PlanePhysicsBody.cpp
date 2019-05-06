#include "rzpch.h"
#include "PlanePhysicsBody.h"

namespace Razor
{

	PlanePhysicsBody::PlanePhysicsBody(const glm::vec3& normal, float constant, const glm::vec3& position) :
		PhysicsBody(position),
		normal(normal),
		constant(constant)
	{
		shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), constant);
	}

	PlanePhysicsBody::~PlanePhysicsBody()
	{
	}

	void PlanePhysicsBody::init()
	{
		btVector3 inertia(0, 0, 0);
		((btStaticPlaneShape*)shape)->calculateLocalInertia(mass, inertia);
		body = new btRigidBody({ mass, motion_state, (btStaticPlaneShape*)shape, inertia });

		body->setRestitution(1.0f);
		body->setFriction(0.5f);
		body->setRollingFriction(0.5f);

		initialized = true;
	}

}

