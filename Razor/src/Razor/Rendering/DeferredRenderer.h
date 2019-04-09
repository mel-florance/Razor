#pragma once

#include "Razor/Materials/ShadersManager.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"

namespace Razor {

	class SceneGraph;

	class DeferredRenderer
	{
	public:
		enum class ClearType {
			ALL,
			COLOR,
			DEPTH
		};

		DeferredRenderer(SceneGraph* sceneGraph);
		~DeferredRenderer();

		void update(double delta);
		void render();
		void clear(ClearType type = ClearType::ALL);
		void enableDepthTest();
		void disableDepthTest();
		void setViewport(unsigned int x, unsigned int y, float w, float h);
		inline TextureAttachment* getColorBuffer() { return colorbuffer; }

	private:
		glm::vec4 color;
		glm::vec2 framebuffer_size;

		ShadersManager* shadersManager;
		SceneGraph* sceneGraph;
		std::shared_ptr<Shader> defaultShader;
		std::shared_ptr<Shader> viewportShader;

		FrameBuffer* framebuffer;
		TextureAttachment* colorbuffer;

		Plane* quad;
	};

}
