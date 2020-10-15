#pragma once

#include "Razor/Materials/Shader.h"

namespace Razor
{
	class Window;
	class Engine;
	class ScenesManager;
	class ShadersManager;
	class GBuffer;
	class Light;
	class RenderPass;
	class Quad;
	class Cube;
	class UVSphere;
	class Texture;
	class TextureAttachment;
	class FrameBuffer;
	class PBRPipeline;
	class Node;

	class DeferredRenderer
	{
	public:
		DeferredRenderer(
			Window* window,
			Engine* engine,
			ScenesManager* scenesManager,
			ShadersManager* shadersManager
		);

		~DeferredRenderer();

		static Shader* deferred_shader;
		static ShadersManager* shadersManager;

		static int num_directional_lights;
		static int num_point_lights;
		static int num_spot_lights;

		inline static void updateLightConstant(const std::string& name, int value)
		{
			if (deferred_shader != nullptr) {
				deferred_shader->defineConstant(Shader::State::FRAGMENT, name, value);
				deferred_shader->replaceConstants();
				deferred_shader->compile();
				deferred_shader->link();
			}
		}

		inline static void incrementDirectionalLights()
		{
			num_directional_lights++;
			updateLightConstant("MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void decrementDirectionalLights()
		{
			num_directional_lights--;

			if (num_directional_lights < 1)
				num_directional_lights = 1;

			updateLightConstant("MAX_DIRECTIONAL_LIGHTS", num_directional_lights);
		}

		inline static void incrementPointLights()
		{
			num_point_lights++;
			updateLightConstant("MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void decrementPointLights()
		{
			num_point_lights--;

			if (num_point_lights < 1)
				num_point_lights = 1;

			updateLightConstant("MAX_POINT_LIGHTS", num_point_lights);
		}

		inline static void incrementSpotLights()
		{
			num_spot_lights++;
			updateLightConstant("MAX_SPOT_LIGHTS", num_spot_lights);
		}

		inline static void decrementSpotLights()
		{
			num_spot_lights--;

			if (num_spot_lights < 1)
				num_spot_lights = 1;

			updateLightConstant("MAX_SPOT_LIGHTS", num_spot_lights);
		}

		enum ClearType
		{
			ALL = 0x0,
			DEPTH = 0x00000100,
			STENCIL = 0x00000400,
			COLOR = 0x00004000
		};

		void onResize(const glm::vec2& size);
		void clear(int flags);
		void setClearColor(const glm::vec4& color);
		void render();
		inline GBuffer* getGBuffer() { return g_buffer; }
		inline PBRPipeline* getPBRPipeline() { return pbr_pipeline; }
		void bindLights(Shader* shader, const std::vector<std::shared_ptr<Light>>& lights);
		std::string formatParamName(const std::string& type, unsigned int index, const std::string& name);

		void drawNode(std::shared_ptr<Node> node, Shader* shader, glm::mat4 parent);

	private:
		void geometryPass();
		void lightingPass();

		void setup_deferred_shaders();
		void setup_framebuffers();

		Window* window;
		Engine* engine;
		ScenesManager* scenesManager;

		glm::ivec2 render_size;
		GBuffer* g_buffer;
		Shader* geometry_shader;
		Shader* fbo_debug_shader;
		Quad* quad;
		Cube* cube;
		UVSphere* sphere;
		PBRPipeline* pbr_pipeline;
	};

}
