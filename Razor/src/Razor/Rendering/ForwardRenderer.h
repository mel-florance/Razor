#pragma once

#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Lighting/Spot.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Materials/Texture.h"

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
	class ColorMaterial;
	class LandscapeMaterial;
	class PlanePhysicsBody;
	class SpherePhysicsBody;
	class CubePhysicsBody;
	class ScenesManager;
	class ShadersManager;
	class VideoTexture;
	class CubemapTexture;
	class BillboardManager;

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
		void renderLineMesh(std::shared_ptr<Node> node, bool isBoundingBox = false);
		void renderOutlines();
		void renderChildOutlines(const glm::mat4& parent, std::shared_ptr<Node> node);
		inline static void addBoundingBox(std::shared_ptr<Node> aabb) { bounding_boxes.push_back(aabb); }
		static void removeBoundingBox(unsigned int node_id);

		static void updateVideoTextures();
		inline static void addVideoTexture(VideoTexture* texture) { video_textures.push_back(texture); }
		static std::vector<VideoTexture*> video_textures;

		void clear(ClearType type = ClearType::ALL);
		void enableDepthTest();
		void disableDepthTest();
		void setViewport(unsigned int x, unsigned int y, float w, float h);
		void setupShaders();
		std::vector<std::shared_ptr<Node>> sortRenderGraph();
		void walkRenderGraph(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& sorted);

		static Shader* defaultShader;
		static Shader* landscapeShader;;

		static int num_directional_lights;
		static int num_point_lights;
		static int num_spot_lights;

		inline static std::shared_ptr<ColorMaterial> getColorMaterial() { return colorMaterial; }

		inline static void updateLightConstant(Shader::State shader_type, const std::string& name, int value)
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
			updateLightConstant(Shader::State::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void decrementDirectionalLights()
		{
			num_directional_lights--;

			if (num_directional_lights < 1)
				num_directional_lights = 1;

			updateLightConstant(Shader::State::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void incrementPointLights()
		{
			num_point_lights++;
			updateLightConstant(Shader::State::FRAGMENT, "MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void decrementPointLights()
		{
			num_point_lights--;

			if (num_point_lights < 1)
				num_point_lights = 1;

			updateLightConstant(Shader::State::FRAGMENT, "MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void incrementSpotLights()
		{
			num_spot_lights++;
			updateLightConstant(Shader::State::FRAGMENT, "MAX_SPOT_LIGHTS", num_spot_lights);
		}

		inline static void decrementSpotLights()
		{
			num_spot_lights--;

			if (num_spot_lights < 1)
				num_spot_lights = 1;

			updateLightConstant(Shader::State::FRAGMENT, "MAX_SPOT_LIGHTS", num_spot_lights);
		}

		TextureAttachment* getColorBuffer();
		TextureAttachment* getDepthBuffer();

		inline static void addLineMesh(std::shared_ptr<Node> node, int priority) { line_meshes.push_back(node); }
		inline void removeLineMesh(std::shared_ptr<Node> node) {
			line_meshes.erase(std::remove(line_meshes.begin(), line_meshes.end(),  node), line_meshes.end());
		}
		inline bool hasLineMesh(std::shared_ptr<Node> node) {
			return std::find(line_meshes.begin(), line_meshes.end(), node) != line_meshes.end();
		}

		static glm::vec2 extractLatLng(const std::string& str);

		static std::vector<std::shared_ptr<Node>> bounding_boxes;
		static std::shared_ptr<ColorMaterial> colorMaterial;
		static glm::vec4 clear_color;

		static std::vector<std::shared_ptr<Node>> line_meshes;
		static std::unique_ptr<BillboardManager> billboard_manager;
		static glm::vec2 geo_offset;
		static glm::vec2 geo_coords;

		static void getISSGeodesicCoords(glm::vec2& position);
		static std::string iss_html; 
		static glm::vec2 iss_position; 

		static int outline_width;
		static glm::vec4 outline_color;

		void updateLightViewMatrix(const glm::vec3& direction, const glm::vec3& center);
		void updateLightOrthoMatrix(float width, float height, float length);

	private:
		Engine* engine;
		Window* window;

		glm::vec2 framebuffer_size;

		ScenesManager* scenesManager;
		ShadersManager* shadersManager;

		std::shared_ptr<Light> light;

		Shader* blurShader;
		Shader* viewportShader;
		Shader* particleShader;
		Shader* gridShader;
		Shader* depthShader;
		Shader* debugDepthShader;
		Shader* skyboxShader;
		Shader* atmosphereShader;
		Shader* outlineShader;

		FrameBuffer* framebuffer;
		FrameBuffer* depthbuffer;
		FrameBuffer* blurbuffer;
		TextureAttachment* colorbuffer;
		TextureAttachment* blur_attachment;
		TextureAttachment* depth_attachment;

		FrameBuffer* selection_buffer;
		TextureAttachment* selection_mask;

		std::shared_ptr<Node> nodePlane;

		Raycast* raycast;

		std::shared_ptr<Quad> quad;

		std::shared_ptr<Point> pointLight;
		std::shared_ptr<Directional> directionalLight;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightProjection;
		glm::mat4 lightView;

		std::shared_ptr<PhongMaterial> defaultMaterial;

		float angle;
		float deltaTime;
		float time;

		PlanePhysicsBody* plane_body;
		std::vector<glm::mat4> modelMatrices;

		VideoTexture* video_texture;
		std::thread video_thread;

		std::shared_ptr<Node> nodeCube;
		std::shared_ptr<Cube> cube;
		CubemapTexture* cubemap;

		std::shared_ptr<Node> nodeSphere;
		std::shared_ptr<UVSphere> sphere;

		std::shared_ptr<Node> nodeDirectional;
		std::shared_ptr<Node> node_satellite;
		std::shared_ptr<Node> earth_node;

		std::shared_ptr<Bounding> light_bounding_debug;
		std::shared_ptr<Node> light_bounding_node;

		glm::mat4 lightViewMatrix;
		glm::mat4 lightProjMatrix;
		glm::mat4 lightProjViewMatrix;
	};

}
