#include "rzpch.h"
#include "ShadowGenerator.h"
#include "ShadowCascade.h"
#include "Razor/Buffers/FrameBuffer.h"
#include "Razor/Buffers/TextureAttachment.h"
#include "Razor/Materials/Shader.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Scene/Scene.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Razor
{

	ShadowGenerator::ShadowGenerator(Camera* camera, const glm::vec2& size) :
		camera(camera),
		size(size),
		bias(0.01f),
		pcf_samples(3),
		alpha(0.172f),
		depth_buffer(nullptr),
		cascades({}),
		cascades_count(3)
	{
		depth_buffer = new FrameBuffer();

		cascades_splits = {
			camera->getClipFar() / 20.0f,
			camera->getClipFar() / 10.0f,
			camera->getClipFar()
		};

		float clip_near = camera->getClipNear();

		for (unsigned int i = 0; i < cascades_count; i++)
		{
			glm::vec2 clip = glm::vec2(clip_near, cascades_splits[i]);
			ShadowCascade* cascade = new ShadowCascade(depth_buffer, size, clip);
			cascades.push_back(cascade);

			clip_near = cascades_splits[i];
		}
	}

	ShadowGenerator::~ShadowGenerator()
	{
		delete depth_buffer;
	}

	void ShadowGenerator::update(const glm::mat4& view_matrix, std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Directional> directional = nullptr;

		for (auto light : scene->getLights())
			directional = std::dynamic_pointer_cast<Directional>(light);

		if (directional != nullptr)
		{
			for (unsigned int i = 0; i < cascades_count; i++)
				cascades[i]->update(camera, view_matrix, directional);
		}
	}

	void ShadowGenerator::render(Scene* scene)
	{
		//depth_buffer->bind();

		//glViewport(0, 0, size.x, size.y);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//depth_shader->bind();

		//for (unsigned int i = 0; i < cascades_count; i++)
		//{
		//	depth_shader->setUniformMat4f("orthoProjectMatrix", cascades[i]->getOrthoProjMatrix());
		//	depth_shader->setUniformMat4f("lightViewMatrix", cascades[i]->getLightViewMatrix());

		//	depth_buffer->updateTextureAttachment(true);
		//	glClear(GL_DEPTH_BUFFER_BIT);
		//}

		//depth_shader->unbind();
		//depth_buffer->unbind();
	}

}
