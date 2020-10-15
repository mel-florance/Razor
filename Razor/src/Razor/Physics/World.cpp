#include "rzpch.h"
#include "World.h"
#include "PhysicsBody.h"
#include "Razor/Scene/Node.h"
#include "Razor/Cameras/Camera.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Razor/Geometry/Geometry.h"
#include "Razor/Rendering/ForwardRenderer.h"
#include "Razor/Materials/Presets/ColorMaterial.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

namespace Razor
{

	World::World() :
		delta(0.0f),
		gravity(glm::vec3(0.0f, -9.80665f, 0.0f)),
		debug_ray_trace_lines(false)
	{
		config = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(config);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();

		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
		world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
		debug_lines_mat = std::make_shared<ColorMaterial>();
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

	void World::updateNodes()
	{
		std::vector<std::shared_ptr<Node>>::iterator node_it = nodes.begin();

		for (; node_it != nodes.end(); ++node_it)
		{
			std::vector<std::shared_ptr<StaticMesh>>::iterator mesh_it = (*node_it)->meshes.begin();

			for (; mesh_it != (*node_it)->meshes.end(); mesh_it++)
			{
				if ((*mesh_it)->getPhysicsEnabled() && (*mesh_it)->getPhysicsBody() != nullptr)
				{
					btMotionState* mesh_motion_state = (*mesh_it)->getPhysicsBody()->getBody()->getMotionState();

					if (mesh_motion_state != nullptr)
					{
						(*node_it)->transform = getMotionStateTransform(mesh_motion_state);

						if ((*mesh_it)->getBoundingMesh() != nullptr && (*mesh_it)->isBoundingBoxVisible())
							(*mesh_it)->updateBoundings((*node_it)->transform);

						for (auto i : (*mesh_it)->getInstances())
						{
							if (i->body != nullptr)
							{
								if (i->body->initialized)
								{
									btMotionState* instance_motion_state = i->body->getBody()->getMotionState();

									if (instance_motion_state != nullptr)
									{
								
										Transform t = getMotionStateTransform(instance_motion_state);
										(*mesh_it)->updateInstance(t.getMatrix(), i->index);
									}
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
		Transform final_transform = Transform();
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

	void World::raycast(RaycastResult* result, Camera* camera, const glm::vec2& mouse, glm::vec2& viewport, float distance)
	{
		float mx = mouse.x / (viewport.x * 0.5f) - 1.0f;
		float my = mouse.y / (viewport.y * 0.5f) - 1.0f;

		glm::mat4 inverse = glm::inverse(camera->getProjectionMatrix() * camera->getViewMatrix());
		glm::vec4 screen = glm::vec4(mx, my, 1.0f, 1.0f);

		glm::vec3 direction = glm::normalize(glm::vec3(inverse * screen));

		glm::vec3 start = camera->getPosition();
		glm::vec3 end = start + direction * distance;

		btVector3 origin = btVector3(start.x, start.y, start.z);
		btVector3 target = btVector3(end.x, end.y, end.z);

		btCollisionWorld::ClosestRayResultCallback hit_result(origin, target);
		//hit_result.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
		//hit_result.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

		world->updateAabbs();
		world->computeOverlappingPairs();
		world->rayTest(origin, target, hit_result);

		if (debug_ray_trace_lines)
		{
			/// TODO: optimize this
			std::shared_ptr<Ray> ray = std::make_shared<Ray>(start, end, distance);
			ray->setMaterial(debug_lines_mat);
			std::shared_ptr<Node> ray_node = std::make_shared<Node>();
			ray_node->meshes.push_back(ray);
			ForwardRenderer::addLineMesh(ray_node, 1);
		}

		if (hit_result.hasHit())
		{
			Node* ptr = (Node*)(hit_result.m_collisionObject->getUserPointer());

			result->hit = true;
			result->node = ptr;
			result->hit_point = glm::vec3(hit_result.m_hitPointWorld.x(), hit_result.m_hitPointWorld.y(), hit_result.m_hitPointWorld.z());
			result->hit_normal = glm::vec3(hit_result.m_hitNormalWorld.x(), hit_result.m_hitNormalWorld.y(), hit_result.m_hitNormalWorld.z());
		}
		else
			result->hit = false;
	}

}
