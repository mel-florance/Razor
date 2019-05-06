#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

namespace Razor
{

	class PhysicsBody
	{
	public:
		PhysicsBody(const glm::vec3& pos = glm::vec3(0.0f));
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

		btTransform transform;
		btDefaultMotionState* motion_state;
		BodyInfos infos;
		btRigidBody* body;
		btCollisionShape* shape;
		bool initialized;
		float mass;
	};

}

