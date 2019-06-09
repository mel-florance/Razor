#include "rzpch.h"
#include "SpherePhysicsBody.h"

namespace Razor
{

	SpherePhysicsBody::SpherePhysicsBody(Node* node, float radius, const glm::vec3& position, const glm::vec3& rotation) :
		PhysicsBody(node, position, rotation),
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
		btScalar linear_damping = 0.5f;
		btScalar angular_damping = 0.5f;
		((btSphereShape*)shape)->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo shape_data = btRigidBody::btRigidBodyConstructionInfo(mass, motion_state, (btSphereShape*)shape, inertia);
		shape_data.m_linearDamping = linear_damping;
		shape_data.m_angularDamping = angular_damping;
		body = new btRigidBody(shape_data);

		body->setUserPointer((void*)user_ptr);
		body->setDamping(linear_damping, angular_damping);
		body->setRestitution(0.1f);
		body->setFriction(0.5f);
		body->setRollingFriction(0.5f);
		//body->setActivationState(DISABLE_DEACTIVATION);
		body->setMassProps(mass, inertia);

		initialized = true;
	}

}
