#include "rzpch.h"
#include "PhysicsBody.h"

namespace Razor
{

	PhysicsBody::PhysicsBody(Node* node, const glm::vec3& pos, const glm::vec3& rot) :
		initialized(false),
		mass(1.0f),
		initial_position(pos),
		user_ptr(node)
	{
		transform = new btTransform();
		transform->setIdentity();
		transform->setOrigin(btVector3(pos.x, pos.y, pos.z));
		transform->setRotation(btQuaternion(rot.x, rot.y, rot.z));
		motion_state = new btDefaultMotionState(*transform);
	}

	PhysicsBody::~PhysicsBody()
	{
		delete motion_state;
		delete transform;
		delete shape;
		delete body;
	}

	void PhysicsBody::setPosition(const glm::vec3& pos)
	{
		//btTransform t;
		//infos.motion_state->getWorldTransform(t);
		//t.setOrigin(btVector3(pos.x, pos.y, pos.z));
		//body->setWorldTransform(t);
	}

}
