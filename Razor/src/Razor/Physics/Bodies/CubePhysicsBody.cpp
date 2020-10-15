#include "rzpch.h"
#include "CubePhysicsBody.h"
#include "Razor/Scene/Node.h"

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

	void CubePhysicsBody::init()
	{
		delete body;

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
	void CubePhysicsBody::updateTransform()
	{
		btTransform& initialTransform = user_ptr->meshes[0]->getPhysicsBody()->getBody()->getWorldTransform();
		delete shape;

		shape = new btBoxShape(btVector3(user_ptr->transform.getScale().x, user_ptr->transform.getScale().y, user_ptr->transform.getScale().z));

		initialTransform.setFromOpenGLMatrix(&user_ptr->transform.getMatrix()[0][0]);

		user_ptr->meshes[0]->getPhysicsBody()->getBody()->setWorldTransform(initialTransform);
		user_ptr->meshes[0]->getPhysicsBody()->getMotionState()->setWorldTransform(initialTransform);

		user_ptr->meshes[0]->getPhysicsBody()->init();
	}
}
