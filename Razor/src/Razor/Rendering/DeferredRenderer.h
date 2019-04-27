#pragma once

#include "Razor/Materials/ShadersManager.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Materials/Texture.h"

#include "Pipeline.h"

namespace Razor {

	class ScenesManager;
	class FPSCamera;
	class TPSCamera;
	class Node;
	class Window;

	class DeferredRenderer
	{
	public:
		enum class ClearType {
			ALL,
			COLOR,
			DEPTH
		};

		DeferredRenderer(Window* window, ScenesManager* scenesManager);
		~DeferredRenderer();

		void update(float delta);
		void render();
		void renderNode(Node* node, glm::mat4 parent);
		void clear(ClearType type = ClearType::ALL);
		void enableDepthTest();
		void disableDepthTest();
		void setViewport(unsigned int x, unsigned int y, float w, float h);
		TextureAttachment* getColorBuffer();

	private:
		glm::vec4 color;
		glm::vec2 framebuffer_size;
		std::vector<Pipeline*> pipelines;

		ShadersManager* shadersManager;
		ScenesManager* scenesManager;
		TPSCamera* tps_camera;
		FPSCamera* fps_camera;
		Window* window;

		Light* light;

		std::shared_ptr<Shader> defaultShader;
		std::shared_ptr<Shader> viewportShader;

		FrameBuffer* framebuffer;
		FrameBuffer* depthbuffer;
		TextureAttachment* colorbuffer;
		TextureAttachment* depth_attachment;

		Quad* quad;

		Node* nodeSphere;

		std::vector<glm::vec3> pointLightPositions;
		Texture* diffuseMap;
		Texture* specularMap;

		float angle;
		float deltaTime;
	};

}
