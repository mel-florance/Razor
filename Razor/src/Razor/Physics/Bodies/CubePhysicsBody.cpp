#include "rzpch.h"
#include "CubePhysicsBody.h"

namespace Razor
{

	CubePhysicsBody::CubePhysicsBody(
		Node* node,
		const glm::vec3& extents, 
		const glm::vec3& position,
		const glm::vec3& rotation
	) : 
		PhysicsBody(node, position, rotation),
		extents(extents)
	{
		shape = new btBoxShape(btVector3(extents.x, extents.y, extents.z));
	}

	CubePhysicsBody::~CubePhysicsBody()
	{
	}

	void CubePhysicsBody::init()
	{
		btVector3 inertia;
		btScalar linear_damping = 0.5f;
		btScalar angular_damping = 0.5f;
		((btBoxShape*)shape)->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo shape_data = btRigidBody::btRigidBodyConstructionInfo(mass, motion_state, (btBoxShape*)shape, inertia);
		shape_data.m_linearDamping = linear_damping;
		shape_data.m_angularDamping = angular_damping;
		body = new btRigidBody(shape_data);

		body->setUserPointer((void*)user_ptr);
		//body->setActivationState(DISABLE_DEACTIVATION);
		body->setMassProps(mass, inertia);

		//body->setDamping(linear_damping, angular_damping);
		//body->setRestitution(1.0f);
		//body->setFriction(0.0f);
		//body->setRollingFriction(0.0f);
		//body->setSpinningFriction(0.0f);

		initialized = true;
	}
}
