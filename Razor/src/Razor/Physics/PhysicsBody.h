#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

namespace Razor
{
	class Node;

	class PhysicsBody
	{
	public:
		PhysicsBody(Node* node, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f));
		~PhysicsBody();

		struct BodyInfos
		{
			btScalar mass;
			btMotionState* motion_state;
			void* shape;
		};

		virtual void init() {}

		void setPosition(const glm::vec3& pos);
		inline btRigidBody* getBody() { return body; }

		glm::vec3 initial_position;
		btTransform* transform;
		btDefaultMotionState* motion_state;
		BodyInfos infos;
		btRigidBody* body;
		btCollisionShape* shape;
		bool initialized;
		float mass;
		Node* user_ptr;
	};

}

