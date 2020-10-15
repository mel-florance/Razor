#pragma once

namespace Razor
{

	class Cube;
	class Quad;
	class Shader;
	class ShadersManager;
	class EnvironmentTexture;

	class PBRPipeline
	{
	public:
		PBRPipeline(ShadersManager* shadersManager);
		~PBRPipeline();

		void setup_shaders();
		void setup_buffers();
		
		inline void setEnvironmentTexture(EnvironmentTexture* texture) { env_texture = texture; }
		void updateEnvironment();

		void setup_environment_map();
		void convert_environment_map();
		void create_irradiance_cubemap();
		void solve_diffuse_integral_convolution();
		void create_prefilter_cubemap();
		void monte_carlo_simulation();
		void generate_lut_from_brdf();

		inline Shader* getShaderPBR() { return shader_pbr; }
		inline Shader* getShaderBackground() { return shader_background; }

		inline unsigned int getIrradianceMap() { return irradiance_map; }
		inline unsigned int getPrefilterMap() { return prefilter_map; }
		inline unsigned int getBrdfLutTexture() { return brdfLUTTexture; }
		inline unsigned int getEnvCubemap() { return env_cubemap; }
		inline EnvironmentTexture* getEnvTexture() { return env_texture; }

	private:
		ShadersManager* shaders_manager;
		EnvironmentTexture* env_texture;

		Cube* cube;
		Quad* quad;

		Shader* shader_pbr;
		Shader* shader_cubemap;
		Shader* shader_irradiance;
		Shader* shader_prefilter;
		Shader* shader_brdf;
		Shader* shader_background;

		unsigned int frame_buffer;
		unsigned int render_buffer;
		unsigned int env_cubemap;
		unsigned int irradiance_map;
		unsigned int prefilter_map;
		unsigned int brdfLUTTexture;

		glm::mat4 capture_projection;
		std::vector<glm::mat4> capture_views;
	};

}
