#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Razor
{
	class Node;
	class Transform;

	class PhysicsBody
	{
	public:
		PhysicsBody(Node* node, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f));
		virtual ~PhysicsBody();

		struct BodyInfos
		{
			btScalar mass;
			btMotionState* motion_state;
			void* shape;
		};

		virtual void init() {}
		virtual void updateTransform();

		void setPosition(const glm::vec3& pos);
		inline btRigidBody* getBody() { return body; }
		inline btDefaultMotionState* getMotionState() { return motion_state; }

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

