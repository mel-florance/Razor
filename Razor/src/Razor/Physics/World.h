#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Razor/Core/Transform.h"

class btMotionState;

namespace Razor
{
	class PhysicsBody;
	class Node;
	class Camera;

	class World
	{
	public:
		World();
		~World();

		void tick(float dt);

		inline glm::vec3& getGravity() { return gravity; }
		inline std::vector<std::shared_ptr<Node>>& getNodes() { return nodes; }

		void setGravity(const glm::vec3& gravity);
		void addNode(std::shared_ptr<Node> node);
		void removeNode(std::shared_ptr<Node> node);
		void updateNodes();
		btDynamicsWorld* getWorld() { return world; }
		
		Transform getMotionStateTransform(btMotionState* motion_state);
		void raycast(Camera* camera, const glm::vec3& start, const glm::vec3& end);

	private:
		float delta;
		btDynamicsWorld* world;
		btDispatcher* dispatcher;
		btAxisSweep3* broadphase;
		btSequentialImpulseConstraintSolver* solver;
		btCollisionConfiguration* config;

		glm::vec3 gravity;

		std::vector<std::shared_ptr<Node>> nodes;
	};

}
