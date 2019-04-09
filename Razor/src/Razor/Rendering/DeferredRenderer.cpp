#include "rzpch.h"

#include "DeferredRenderer.h"
#include "glad/glad.h"

#include "Razor/Scene/SceneGraph.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"

namespace Razor {

	DeferredRenderer::DeferredRenderer(SceneGraph* sceneGraph) : sceneGraph(sceneGraph)
	{
		shadersManager = new ShadersManager();

		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		framebuffer_size = { 1280, 720 };
	
		defaultShader = shadersManager->createShader("default");
		viewportShader = shadersManager->createShader("viewport");

		viewportShader->bind();
		viewportShader->setUniform1i("target", 0);

		quad = new Plane();

		framebuffer = new FrameBuffer();
		colorbuffer = framebuffer->addTextureAttachment(framebuffer_size);
		framebuffer->addRenderBufferAttachment(framebuffer_size);

		Node* node = new Node();
		Cube* cube = new Cube();
		node->meshes.push_back(cube);
		sceneGraph->addNode(node);
	}

	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::update(double delta) 
	{
		
	}

	void DeferredRenderer::render()
	{
		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

		framebuffer->bind();
		enableDepthTest();

		clear();

		defaultShader->bind();

		for (auto node : sceneGraph->getNodes())
		{
			for (auto& mesh : node->meshes)
			{
				mesh->getVao()->bind();
				mesh->draw();
				mesh->getVao()->unbind();
			}
		}

		defaultShader->unbind();
		framebuffer->unbind();
		disableDepthTest();


		// TODO: Do some fullscreen with this.

		/*clear(ClearType::COLOR);

		viewportShader->bind();
		quad->getVao()->bind();
		glBindTexture(GL_TEXTURE_2D, colorbuffer->getId());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		quad->getVao()->unbind();
		viewportShader->unbind();*/
	}

	void DeferredRenderer::clear(ClearType type)
	{
		int flags;
		glClearColor(color.x, color.y, color.z, color.w);

		switch (type) {
		default:
		case ClearType::ALL:
			flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			break;
		case ClearType::COLOR:
			flags = GL_COLOR_BUFFER_BIT;
			break;
		case ClearType::DEPTH:
			flags = GL_DEPTH_BUFFER_BIT;
		}

		glClear(flags);
	}

	void DeferredRenderer::enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::setViewport(unsigned int x, unsigned int y, float w, float h)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

}


