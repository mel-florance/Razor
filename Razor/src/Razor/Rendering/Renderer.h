#pragma once

#include "Razor/Materials/Texture.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Core/Window.h"

namespace Razor
{

	class Node;
	class Shader;
	class Engine;
	class VertexArray;
	class ScenesManager;
	class ShadersManager;
	class ForwardRenderer;
	class DeferredRenderer;
	class TextureAttachment;
	class FrameBuffer;
	class Material;
	class GBuffer;

	class Renderer
	{
	public:
		Renderer(
			Window* window, 
			Engine* engine,
			ScenesManager* scenesManager,
			ShadersManager* shadersManager
		);
		~Renderer();

		enum class RenderCategory
		{
			UI,
			LANDSCAPE,
			OBJECT,
			FOLIAGE,
			SKYBOX,
			PARTICLE
		};

		enum class RenderSortKey
		{
			SHADER,
			MATERIAL,
			MESH,
			TEXTURE
		};

		void update();
		void render();

		void setClearColor(const glm::vec4& color);
		void onResize(const glm::vec2& size);

		inline GBuffer* getGBuffer() { return g_buffer; }

		struct RenderTask
		{
			RenderTask() :
				vao(nullptr),
				shader(nullptr),
				category(RenderCategory::OBJECT),
				uniforms({}),
				sorting_keys({
					RenderSortKey::SHADER,
					RenderSortKey::TEXTURE,
					RenderSortKey::MATERIAL,
					RenderSortKey::MESH
					}),
				draw_mode(StaticMesh::DrawMode::TRIANGLES),
				vertex_count(0),
				index_count(0),
				depth_pass(false)
			{}

			VertexArray* vao;
			Shader* shader;
			RenderCategory category;
			std::unordered_map<const char*, Variant> uniforms;
			std::unordered_map<Texture::Type, unsigned int> textures;
			std::vector<RenderSortKey> sorting_keys;
			StaticMesh::DrawMode draw_mode;
			unsigned int vertex_count;
			unsigned int index_count;
			bool depth_pass;

			friend bool operator < (const RenderTask& lhs, const RenderTask& rhs);
		};

	protected:
		void addRenderTask(std::shared_ptr<Node> node);
		void processQueue();

		std::priority_queue<RenderTask> tasks;
		
		ForwardRenderer* forward;
		DeferredRenderer* deferred;

		GBuffer* g_buffer;

		ScenesManager* scenes_manager;
		ShadersManager* shaders_manager;

		std::unordered_map<Shader::Type, Shader*> internal_shaders;
		glm::vec4 clear_color;
	};

}
