#include "rzpch.h"
#include "PhysicsBody.h"
#include "Razor/Scene/Node.h"

namespace Razor
{

	PhysicsBody::PhysicsBody(Node* node, const glm::vec3& position, const glm::vec3& rotation) :
		initialized(false),
		mass(1.0f),
		initial_position(node->transform.getPosition()),
		user_ptr(node),
		body(nullptr)
	{
		transform = new btTransform();
		transform->setIdentity();

		if (glm::length(position) > 0 && glm::length(rotation) > 0)
		{
			transform->setOrigin(btVector3(position.x, position.y, position.z));
			transform->setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		}
		else
			transform->setFromOpenGLMatrix(&node->transform.getMatrix()[0][0]);

		motion_state = new btDefaultMotionState(*transform);
	}

	PhysicsBody::~PhysicsBody()
	{
		delete motion_state;
		delete transform;
		delete shape;
		delete body;
		//delete user_ptr;
	}

	void PhysicsBody::setPosition(const glm::vec3& pos)
	{
		//btTransform t;
		//infos.motion_state->getWorldTransform(t);
		//t.setOrigin(btVector3(pos.x, pos.y, pos.z));
		//body->setWorldTransform(t);
	}

	void PhysicsBody::updateTransform()
	{
		btTransform initialTransform = user_ptr->meshes[0]->getPhysicsBody()->getBody()->getWorldTransform();

		initialTransform.setFromOpenGLMatrix(&user_ptr->transform.getMatrix()[0][0]);

		user_ptr->meshes[0]->getPhysicsBody()->getBody()->setWorldTransform(initialTransform);
		user_ptr->meshes[0]->getPhysicsBody()->getMotionState()->setWorldTransform(initialTransform);

		user_ptr->meshes[0]->getPhysicsBody()->init();
	}

}
