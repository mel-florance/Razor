#pragma once

#include "Razor/Materials/ShadersManager.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Lighting/Spot.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Materials/Texture.h"

#include "Pipeline.h"

namespace Razor
{

	class Window;
	class Engine;
	class ScenesManager;
	class FPSCamera;
	class TPSCamera;
	class Node;
	class Raycast;
	class Mesh;
	class PhongMaterial;
	class PlanePhysicsBody;
	class SpherePhysicsBody;

	class DeferredRenderer
	{
	public:
		enum class ClearType {
			ALL,
			COLOR,
			DEPTH,
			STENCIL
		};

		DeferredRenderer(Window* window, Engine* engine, ScenesManager* scenesManager);
		~DeferredRenderer();

		void update(float delta);
		void render();
		void renderNode(Shader* shader, Node* node, glm::mat4 parent, bool depth = false);
		void renderParticleSystems();
		void renderLineMesh(Node* node);
		void clear(ClearType type = ClearType::ALL);
		void enableDepthTest();
		void disableDepthTest();
		void setViewport(unsigned int x, unsigned int y, float w, float h);
		TextureAttachment* getColorBuffer();

		inline void addLineMesh(Node* node, int priority) { line_meshes.push_back(node); }
		inline void removeLineMesh(Node* node) {
			line_meshes.erase(std::remove(line_meshes.begin(), line_meshes.end(),  node), line_meshes.end());
		}
		inline bool hasLineMesh(Node* node) {
			return std::find(line_meshes.begin(), line_meshes.end(), node) != line_meshes.end();
		}

	private:
		Engine* engine;
		Window* window;

		glm::vec4 color;
		glm::vec2 framebuffer_size;
		std::vector<Pipeline*> pipelines;

		ShadersManager* shadersManager;
		ScenesManager* scenesManager;

		Light* light;

		Shader* defaultShader;
		Shader* viewportShader;
		Shader* particleShader;
		Shader* gridShader;
		Shader* depthShader;
		Shader* debugDepthShader;

		FrameBuffer* framebuffer;
		FrameBuffer* depthbuffer;
		TextureAttachment* colorbuffer;
		TextureAttachment* depth_attachment;

		Quad* quad;

		Node* nodeSphere;
		Node* nodePlane;

		Raycast* raycast;

		Texture* diffuseMap;
		Texture* specularMap;
		Texture* checkerMap;

		Grid* grid;
		Node* grid_node;
		Node* node_axis;
		Ray* axis_x;
		Ray* axis_y;
		Ray* axis_z;

		Point* pointLight;
		Directional* directionalLight;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightProjection;
		glm::mat4 lightView;

		PhongMaterial* defaultMaterial;

		float angle;
		float deltaTime;

		std::vector<Node*> line_meshes;

		PlanePhysicsBody* plane_body;
		SpherePhysicsBody* sphere_body;
	};

}
