#pragma once

#include "Razor/Materials/ShadersManager.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Materials/Texture.h"

namespace Razor {

	class SceneGraph;
	class Camera;

	class DeferredRenderer
	{
	public:
		enum class ClearType {
			ALL,
			COLOR,
			DEPTH
		};

		DeferredRenderer(Camera* camera, SceneGraph* sceneGraph);
		~DeferredRenderer();

		void update(double delta);
		void render(double delta);
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
		Camera* camera;
		Light* light;

		std::shared_ptr<Shader> defaultShader;
		std::shared_ptr<Shader> viewportShader;

		FrameBuffer* framebuffer;
		TextureAttachment* colorbuffer;

		Plane* quad;

		std::vector<glm::vec3> pointLightPositions;
		Texture* diffuseMap;
		Texture* specularMap;

		float angle;
		double deltaTime;
	};

}
