#include "rzpch.h"
#include "CubePhysicsBody.h"

namespace Razor
{

	CubePhysicsBody::CubePhysicsBody(const glm::vec3& extents) : 
		PhysicsBody(),
		extents(extents)
	{
		shape = new btBoxShape(btVector3(extents.x, extents.y, extents.z));
	}

	CubePhysicsBody::~CubePhysicsBody()
	{
	}

	void CubePhysicsBody::init()
	{
		btVector3 inertia(0, 0, 0);
		((btBoxShape*)shape)->calculateLocalInertia(mass, inertia);
		body = new btRigidBody({ mass, motion_state, (btBoxShape*)shape, inertia });

		body->setRestitution(1.0f);
		body->setFriction(0.5f);
		body->setRollingFriction(0.5f);
		body->setActivationState(DISABLE_DEACTIVATION);

		initialized = true;
	}
}
