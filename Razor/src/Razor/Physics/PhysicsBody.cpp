#include "rzpch.h"
#include "PhysicsBody.h"

namespace Razor
{

	PhysicsBody::PhysicsBody(const glm::vec3& pos) :
		initialized(false),
		mass(0.0f)
	{
		transform.setIdentity();
		transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
		motion_state = new btDefaultMotionState(transform);
	}

	PhysicsBody::~PhysicsBody()
	{
		delete motion_state;
	}

	void PhysicsBody::setPosition(const glm::vec3& pos)
	{
		transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	}

}
