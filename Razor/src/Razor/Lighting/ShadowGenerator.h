#pragma once

namespace Razor
{

	class FrameBuffer;
	class TextureAttachment;
	class ShadowFrustum;
	class ShadowCascade;
	class Camera;
	class Directional;
	class Scene;

	class ShadowGenerator
	{
	public:
		ShadowGenerator(Camera* camera, const glm::vec2& size = glm::vec2(2048.0f));
		~ShadowGenerator();

		void update(const glm::mat4& view_matrix, std::shared_ptr<Scene> scene);
		void render(Scene* scene);

		inline void setCascadesCount(unsigned int count) { cascades_count = count; }
		inline void setCamera(Camera* camera) { this->camera = camera; }

		inline glm::vec2& getSize() { return size; }
		inline FrameBuffer* getDepthBuffer() { return depth_buffer; }
		inline unsigned int& getCascadesCount() { return cascades_count; }
		inline std::vector<ShadowCascade*>& getCascades() { return cascades; }
		inline float& getBias() { return bias; }
		inline int& getPcfSamples() { return pcf_samples; }
		inline float& getAlpha() { return alpha; }
		inline std::array<float, 3>& getCascadesSplits() { return cascades_splits; }

	private:
		glm::vec2 size;

		Camera* camera;
		FrameBuffer* depth_buffer;

		unsigned int cascades_count;
		std::array<float, 3> cascades_splits;
		std::vector<ShadowCascade*> cascades;

		float bias;
		int pcf_samples;
		float alpha;
	};

}
