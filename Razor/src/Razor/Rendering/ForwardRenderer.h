#pragma once

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

	class Event;
	class Window;
	class Engine;
	class FPSCamera;
	class TPSCamera;
	class Node;
	class Raycast;
	class Mesh;
	class PhongMaterial;
	class LandscapeMaterial;
	class PlanePhysicsBody;
	class SpherePhysicsBody;
	class CubePhysicsBody;
	class ScenesManager;
	class ShadersManager;

	class ForwardRenderer
	{
	public:
		enum class ClearType 
		{
			ALL,
			COLOR,
			DEPTH,
			STENCIL
		};

		ForwardRenderer(Window* window, Engine* engine, ScenesManager* scenesManager, ShadersManager* shadersManager);
		~ForwardRenderer();

		void update(float delta);
		void onEvent(Event& event);

		void render();
		void renderNode(Shader* shader, std::shared_ptr<Node> node, glm::mat4 parent, bool depth = false);
		void renderParticleSystems();
		void renderLineMesh(std::shared_ptr<Node> node);

		void clear(ClearType type = ClearType::ALL);
		void enableDepthTest();
		void disableDepthTest();
		void setViewport(unsigned int x, unsigned int y, float w, float h);
		void setupShaders();

		static Shader* defaultShader;
		static Shader* landscapeShader;;

		static int num_directional_lights;
		static int num_point_lights;
		static int num_spot_lights;

		inline static void updateLightConstant(Shader::Type shader_type, const std::string& name, int value)
		{
			defaultShader->defineConstant(shader_type, name, value);
			defaultShader->replaceConstants();
			defaultShader->compile();
			defaultShader->link();

			landscapeShader->defineConstant(shader_type, name, value);
			landscapeShader->replaceConstants();
			landscapeShader->compile();
			landscapeShader->link();
		}

		inline static void incrementDirectionalLights()
		{
			num_directional_lights++;
			updateLightConstant(Shader::Type::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void decrementDirectionalLights()
		{
			num_directional_lights--;

			if (num_directional_lights < 1)
				num_directional_lights = 1;

			updateLightConstant(Shader::Type::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void incrementPointLights()
		{
			num_point_lights++;
			updateLightConstant(Shader::Type::FRAGMENT, "MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void decrementPointLights()
		{
			num_point_lights--;

			if (num_point_lights < 1)
				num_point_lights = 1;

			updateLightConstant(Shader::Type::FRAGMENT, "MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void incrementSpotLights()
		{
			num_spot_lights++;
			updateLightConstant(Shader::Type::FRAGMENT, "MAX_SPOT_LIGHTS", num_spot_lights);
		}

		inline static void decrementSpotLights()
		{
			num_spot_lights--;

			if (num_spot_lights < 1)
				num_spot_lights = 1;

			updateLightConstant(Shader::Type::FRAGMENT, "MAX_SPOT_LIGHTS", num_spot_lights);
		}

		TextureAttachment* getColorBuffer();
		TextureAttachment* getDepthBuffer();

		inline void addLineMesh(std::shared_ptr<Node> node, int priority) { line_meshes.push_back(node); }
		inline void removeLineMesh(std::shared_ptr<Node> node) {
			line_meshes.erase(std::remove(line_meshes.begin(), line_meshes.end(),  node), line_meshes.end());
		}
		inline bool hasLineMesh(std::shared_ptr<Node> node) {
			return std::find(line_meshes.begin(), line_meshes.end(), node) != line_meshes.end();
		}

	private:
		Engine* engine;
		Window* window;

		glm::vec4 clear_color;
		glm::vec2 framebuffer_size;
		std::vector<Pipeline*> pipelines;

		ScenesManager* scenesManager;
		ShadersManager* shadersManager;

		std::shared_ptr<Light> light;

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

		std::shared_ptr<Node> nodeSphere;
		std::shared_ptr<Node> nodePlane;
		std::shared_ptr<Node> nodeCube;

		Raycast* raycast;

		std::shared_ptr<Point> pointLight;
		std::shared_ptr<Directional> directionalLight;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightProjection;
		glm::mat4 lightView;

		std::shared_ptr<PhongMaterial> defaultMaterial;
		std::shared_ptr<LandscapeMaterial> landscapeMaterial;

		float angle;
		float deltaTime;

		std::vector<std::shared_ptr<Node>> line_meshes;

		PlanePhysicsBody* plane_body;
		SpherePhysicsBody* sphere_body;
		CubePhysicsBody* cube_body;

		Cube* cube_test;
		std::shared_ptr<StaticMesh> pine_tree;
		std::vector<glm::mat4> modelMatrices;
	};

}
