#pragma once

#define FRUSTUM_CORNERS 8

namespace Razor
{

	class TextureAttachment;
	class Camera;
	class Directional;
	class FrameBuffer;

	class ShadowCascade
	{
	public:
		ShadowCascade(FrameBuffer* frame_buffer, const glm::vec2& size, const glm::vec2& clip_planes);
		~ShadowCascade();

		void update(Camera* camera, const glm::mat4& view_matrix, std::shared_ptr<Directional> light);
		glm::vec3 frustumCorner(const glm::mat4& mat, int corner, glm::vec3& point);
		void updateLightViewMatrix(const glm::vec3& light_direction, const glm::vec3& light_position);
		void updateLightProjectionMatrix();

		inline glm::mat4 getLightViewMatrix() { return light_view_matrix; }
		inline glm::mat4 getOrthoProjMatrix() { return ortho_proj_matrix; }
		inline TextureAttachment* getDepthTexture() { return depth_texture; }

	private:
		FrameBuffer* frame_buffer;
		TextureAttachment* depth_texture;
		glm::vec2 size;
		glm::vec2 clip_planes;
		
		glm::mat4 proj_view_matrix;
		glm::mat4 ortho_proj_matrix;
		glm::mat4 light_view_matrix;

		glm::vec3 centroid;
		std::array<glm::vec3, FRUSTUM_CORNERS> frustum_corners;

		glm::vec4 temp;
	};

}
