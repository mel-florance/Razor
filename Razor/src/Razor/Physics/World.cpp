#include "rzpch.h"
#include "World.h"
#include "PhysicsBody.h"
#include "Razor/Scene/Node.h"
#include "Razor/Cameras/Camera.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Razor
{

	World::World() :
		gravity(glm::vec3(0.0f, -9.81f, 0.0f))
	{
		config = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(config);
		btVector3	worldAabbMin(-1000, -1000, -1000);
		btVector3	worldAabbMax(1000, 1000, 1000);
		broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax);
		solver = new btSequentialImpulseConstraintSolver();

		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
		world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	World::~World()
	{
		delete world;
		delete dispatcher;
		delete broadphase;
		delete solver;
		delete config;
	}

	void World::tick(float dt)
	{
		delta = dt;
		world->stepSimulation(dt);
		updateNodes();
	}

	void World::setGravity(const glm::vec3& gravity)
	{
		this->gravity = gravity;
		world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	void World::addNode(std::shared_ptr<Node> node)
	{
		for (auto mesh : node->meshes)
		{
			mesh->getPhysicsBody()->init();
			world->addRigidBody(mesh->getPhysicsBody()->getBody());
		}

		nodes.push_back(node);
	}

	void World::removeNode(std::shared_ptr<Node> node)
	{
		for (auto mesh : node->meshes)
			if(mesh->getPhysicsBody() != nullptr)
				world->removeRigidBody(mesh->getPhysicsBody()->getBody());

		nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
	}

	struct btDrawingResult : public btCollisionWorld::ContactResultCallback
	{
		virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
		{
			float z, y, x;
			btQuaternion quat = colObj1Wrap->getWorldTransform().getRotation();
			quat.getEulerZYX(z, y, x);
			
			Node* node = static_cast<Node*>(colObj1Wrap->getCollisionShape()->getUserPointer());

			if (node != nullptr)
			{
				node->transform.setRotation(glm::vec3(x, y, z));
				Log::trace("%s", "ok");
			}

			return 0;
		}
	};

	void World::updateNodes()
	{
		for (auto node : nodes)
		{
			for (auto mesh : node->meshes)
			{
				if (mesh->getPhysicsEnabled() && mesh->getPhysicsBody() != nullptr)
				{
					btMotionState* mesh_motion_state = mesh->getPhysicsBody()->getBody()->getMotionState();

					if (mesh_motion_state != nullptr)
					{
						node->transform = getMotionStateTransform(mesh_motion_state);
				
						for (auto i : mesh->getInstances())
						{
							if (i->body->initialized)
							{
								btMotionState* instance_motion_state = i->body->getBody()->getMotionState();

								if (instance_motion_state != nullptr)
								{
									mesh->updateInstance(getMotionStateTransform(instance_motion_state).getMatrix(), i->index);
								}
							}
						}
					}
				}
			}
		}
	}

	Transform World::getMotionStateTransform(btMotionState* motion_state)
	{
		Transform final_transform =Transform();
		btTransform transform;
		motion_state->getWorldTransform(transform);

		btVector3 origin = transform.getOrigin();
		final_transform.setPosition(glm::vec3(origin.getX(), origin.getY(), origin.getZ()));

		btScalar mat[16];
		transform.getOpenGLMatrix(mat);

		glm::mat4 matrix = glm::make_mat4(mat);

		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 persp;

		glm::decompose(matrix, scale, orientation, translation, skew, persp);
		final_transform.setRotation(glm::eulerAngles(orientation));
		final_transform.setScale(scale);

		return final_transform;
	}

	void World::raycast(Camera* camera, const glm::vec3& start, const glm::vec3& end)
	{
		btVector3 origin = btVector3(start.x, start.y, start.z);
		btVector3 target = btVector3(end.x, end.y, end.z);

		btCollisionWorld::AllHitsRayResultCallback hit_result(origin, target);
		world->rayTest(origin, target, hit_result);

		if (hit_result.hasHit())
		{
			Log::trace("hit result !");
		}
	}

}
