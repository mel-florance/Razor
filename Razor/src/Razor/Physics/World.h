#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <Razor/Core/Transform.h>
#include <Razor/Maths/Maths.h>

class btMotionState;

namespace Razor
{
	class PhysicsBody;
	class Node;
	class Camera;
	class ColorMaterial;

	class World
	{
	public:
		World();
		~World();

		struct RaycastResult {
			bool hit;
			Node* node;
			glm::vec3 hit_point;
			glm::vec3 hit_normal;
		};

		void tick(float dt);

		inline glm::vec3& getGravity() { return gravity; }
		inline std::vector<std::shared_ptr<Node>>& getNodes() { return nodes; }

		void setGravity(const glm::vec3& gravity);
		void addNode(std::shared_ptr<Node> node);
		void removeNode(std::shared_ptr<Node> node);
		void updateNodes();
		btDynamicsWorld* getWorld() { return world; }
		
		Transform getMotionStateTransform(btMotionState* motion_state);
		void raycast(RaycastResult* result, Camera* camera, const glm::vec2& mouse, glm::vec2& viewport, float distance);

	private:
		bool debug_ray_trace_lines;
		float delta;
		glm::vec3 gravity;

		btDynamicsWorld* world;
		btDispatcher* dispatcher;
		btDbvtBroadphase* broadphase;
		btSequentialImpulseConstraintSolver* solver;
		btCollisionConfiguration* config;

		std::vector<std::shared_ptr<Node>> nodes;
		std::shared_ptr<ColorMaterial> debug_lines_mat;
	};

}
