#include "rzpch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "PBRPipeline.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Materials/EnvironmentTexture.h"
#include "Razor/Geometry/Geometry.h"

namespace Razor
{

	PBRPipeline::PBRPipeline(ShadersManager* shadersManager) :
		cube(nullptr),
		quad(nullptr),
		shaders_manager(shadersManager)
	{
		cube = new Cube();
		cube->setCulling(false);

		quad = new Quad();

		setup_shaders();
		setup_buffers();
		setup_environment_map();
	}

	PBRPipeline::~PBRPipeline()
	{
		delete cube;
		delete quad;
	}

	void PBRPipeline::setup_shaders()
	{
		shader_pbr        = shaders_manager->getShader("pbr");
		shader_cubemap    = shaders_manager->getShader("cubemap");
		shader_irradiance = shaders_manager->getShader("irradiance");
		shader_prefilter  = shaders_manager->getShader("prefilter");
		shader_brdf       = shaders_manager->getShader("brdf");
		shader_background = shaders_manager->getShader("background");

		shader_pbr->bind();
		shader_pbr->setUniform1i("irradianceMap", 0);
		shader_pbr->setUniform1i("prefilterMap", 1);
		shader_pbr->setUniform1i("brdfLUT", 2);
		shader_pbr->setUniform1i("albedoMap", 3);
		shader_pbr->setUniform1i("normalMap", 4);
		shader_pbr->setUniform1i("metallicMap", 5);
		shader_pbr->setUniform1i("roughnessMap", 6);
		shader_pbr->setUniform1i("aoMap", 7);
		shader_pbr->setUniform1i("ormMap", 8);
		shader_pbr->setUniform1i("opacityMap", 9);
		shader_pbr->setUniform1i("emissiveMap", 10);

		shader_background->bind();
		shader_background->setUniform1i("environmentMap", 0);
	}

	void PBRPipeline::setup_buffers()
	{
		glGenFramebuffers(1, &frame_buffer);
		glGenRenderbuffers(1, &render_buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	void PBRPipeline::setup_environment_map()
	{
		glGenTextures(1, &env_cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		capture_views =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
	}

	void PBRPipeline::updateEnvironment()
	{
		if (env_texture != nullptr)
		{
			convert_environment_map();
			create_irradiance_cubemap();
			solve_diffuse_integral_convolution();
			create_prefilter_cubemap();
			monte_carlo_simulation();
			generate_lut_from_brdf();
		}
	}

	void PBRPipeline::convert_environment_map()
	{
		shader_cubemap->bind();
		shader_cubemap->setUniform1i("equirectangularMap", 0);
		shader_cubemap->setUniformMat4f("projection", capture_projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, env_texture->getId());
		
		glViewport(0, 0, 512, 512);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		for (unsigned int i = 0; i < 6; ++i)
		{
			shader_cubemap->setUniformMat4f("view", capture_views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube->getVao()->bind();
			cube->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void PBRPipeline::create_irradiance_cubemap()
	{
		glGenTextures(1, &irradiance_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 32, 32);
	}

	void PBRPipeline::solve_diffuse_integral_convolution()
	{
		shader_irradiance->bind();
		shader_irradiance->setUniform1i("environmentMap", 0);
		shader_irradiance->setUniformMat4f("projection", capture_projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

		glViewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		for (unsigned int i = 0; i < 6; ++i)
		{
			shader_irradiance->setUniformMat4f("view", capture_views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube->getVao()->bind();
			cube->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PBRPipeline::create_prefilter_cubemap()
	{
		glGenTextures(1, &prefilter_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_map);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void PBRPipeline::monte_carlo_simulation()
	{
		shader_prefilter->bind();
		shader_prefilter->setUniform1i("environmentMap", 0);
		shader_prefilter->setUniformMat4f("projection", capture_projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		unsigned int maxMipLevels = 5;

		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);

			glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			shader_prefilter->setUniform1f("roughness", roughness);

			for (unsigned int i = 0; i < 6; ++i)
			{
				shader_prefilter->setUniformMat4f("view", capture_views[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				cube->getVao()->bind();
				cube->draw();
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PBRPipeline::generate_lut_from_brdf()
	{
		glGenTextures(1, &brdfLUTTexture);

		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

		glViewport(0, 0, 512, 512);
		shader_brdf->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quad->getVao()->bind();
		quad->draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}