#include "rzpch.h"
#include "PlanePhysicsBody.h"

namespace Razor
{

	PlanePhysicsBody::PlanePhysicsBody(
		Node* node,
		const glm::vec3& normal,
		float constant
		) :
		PhysicsBody(node),
		normal(normal),
		constant(constant)
	{
		mass = 0.0f;
		shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), constant);
	}

	void PlanePhysicsBody::init()
	{
		btVector3 inertia;
		btScalar linear_damping = 0.5f;
		btScalar angular_damping = 0.5f;
		((btStaticPlaneShape*)shape)->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo shape_data = btRigidBody::btRigidBodyConstructionInfo(mass, motion_state, (btStaticPlaneShape*)shape, inertia);
		shape_data.m_linearDamping = linear_damping;
		shape_data.m_angularDamping = angular_damping;
		body = new btRigidBody(shape_data);
		
		body->setUserPointer((void*)user_ptr);
		body->setDamping(linear_damping, angular_damping);
		body->setRestitution(0.0f);
		body->setFriction(0.5f);
		body->setRollingFriction(0.5f);
		body->setSpinningFriction(0.5f);

		initialized = true;
	}

}

