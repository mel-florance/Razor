#include "rzpch.h"
#include "ShadowCascade.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Buffers/TextureAttachment.h"
#include "Razor/Buffers/FrameBuffer.h"

namespace Razor
{
	ShadowCascade::ShadowCascade(FrameBuffer* frame_buffer, const glm::vec2& size, const glm::vec2& clip_planes) :
		frame_buffer(frame_buffer),
		size(size),
		clip_planes(clip_planes),
		depth_texture(nullptr),
		proj_view_matrix(glm::mat4(1.0f)),
		ortho_proj_matrix(glm::mat4(1.0f)),
		light_view_matrix(glm::mat4(1.0f)),
		centroid(glm::vec3(0.0f)),
		frustum_corners({ glm::vec3(0.0f) })
	{
		depth_texture = frame_buffer->addTextureAttachment(size, true);
	}

	ShadowCascade::~ShadowCascade()
	{
		delete depth_texture;
	}

	void ShadowCascade::update(Camera* camera, const glm::mat4& view_matrix, std::shared_ptr<Directional> light)
	{
		float aspect = camera->getAspectRatio();

		proj_view_matrix = glm::perspective(camera->getFov(), aspect, clip_planes.x, clip_planes.y);
		proj_view_matrix *= view_matrix;

		float max_z = std::numeric_limits<float>::min();
		float min_z = std::numeric_limits<float>::max();

		for (unsigned int i = 0; i < FRUSTUM_CORNERS; i++)
		{
			frustum_corners[i] = glm::vec3(0.0f);
			frustumCorner(proj_view_matrix, i, frustum_corners[i]);

			centroid += frustum_corners[i];
			centroid /= 8.0f;

			min_z = std::min(min_z, frustum_corners[i].z);
			max_z = std::max(max_z, frustum_corners[i].z);
		}

		glm::vec3 light_dir = light->getDirection();
		glm::vec3 pos = glm::vec3(light_dir);

		float distance = max_z - min_z;
		pos *= distance;
		
		glm::vec3 light_pos = glm::vec3(centroid);
		light_pos += pos;

		updateLightViewMatrix(light_dir, light_pos);
		updateLightProjectionMatrix();
	}

	void ShadowCascade::updateLightViewMatrix(const glm::vec3& light_direction, const glm::vec3& light_position)
	{
		float x = glm::degrees(acos(light_direction.z));
		float y = glm::degrees(asin(light_direction.x));
		float z = 0.0f;

		glm::mat4 rot_x = glm::rotate(glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rot_y = glm::rotate(glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 translation = glm::translate(-light_position);

		light_view_matrix = rot_x * rot_y * translation;
	}

	void ShadowCascade::updateLightProjectionMatrix()
	{
		const float MIN = std::numeric_limits<float>::min();
		const float MAX = std::numeric_limits<float>::max();

		float min_x =  MAX;
		float max_x = -MIN;

		float min_y =  MAX;
		float max_y = -MIN;

		float min_z =  MAX;
		float max_z = -MIN;

		for (unsigned int i = 0; i < FRUSTUM_CORNERS; i++)
		{
			glm::vec3 corner = frustum_corners[i];
			temp = glm::vec4(corner, 1.0f);
			temp = light_view_matrix * temp;

			min_x = std::min(temp.x, min_x);
			max_x = std::max(temp.x, max_x);

			min_y = std::min(temp.y, min_y);
			max_y = std::max(temp.y, max_y);

			min_z = std::min(temp.z, min_z);
			max_z = std::max(temp.z, max_z);
		}

		float distance = max_z - min_z;
		ortho_proj_matrix = glm::ortho(min_x, max_x, min_y, max_y, 0.0f, distance);
	}

	glm::vec3 ShadowCascade::frustumCorner(const glm::mat4& m, int corner, glm::vec3& point)
	{
		float d1 = 0.0f, d2 = 0.0f, d3 = 0.0f;

		float n1x = 0.0f, n1y = 0.0f, n1z = 0.0f,
			  n2x = 0.0f, n2y = 0.0f, n2z = 0.0f,
			  n3x = 0.0f, n3y = 0.0f, n3z = 0.0f;

		switch (corner) {
		case 0:
			n1x = m[0][3] + m[0][0];	n1y = m[1][3] + m[1][0];	n1z = m[2][3] + m[2][0];	d1 = m[3][3] + m[3][0];
			n2x = m[0][3] + m[0][1];	n2y = m[1][3] + m[1][1];	n1z = m[2][3] + m[2][1];	d2 = m[3][3] + m[3][1];
			n3x = m[0][3] + m[0][2];	n3y = m[1][3] + m[1][2];	n3z = m[2][3] + m[2][2];	d3 = m[3][3] + m[3][2];
			break;
		case 1:
			n1x = m[0][3] - m[0][0];	n1y = m[1][3] - m[1][0];	n1z = m[2][3] - m[2][0];	d1 = m[3][3] - m[3][0];
			n2x = m[0][3] + m[0][1];	n2y = m[1][3] + m[1][1];	n1z = m[2][3] + m[2][1];	d2 = m[3][3] + m[3][1];
			n3x = m[0][3] + m[0][2];	n3y = m[1][3] + m[1][2];	n3z = m[2][3] + m[2][2];	d3 = m[3][3] + m[3][2];
			break;
		case 2:
			n1x = m[0][3] - m[0][0];	n1y = m[1][3] - m[1][0];	n1z = m[2][3] - m[2][0];	d1 = m[3][3] - m[3][0];
			n2x = m[0][3] - m[0][1];	n2y = m[1][3] - m[1][1];	n1z = m[2][3] - m[2][1];	d2 = m[3][3] - m[3][1];
			n3x = m[0][3] + m[0][2];	n3y = m[1][3] + m[1][2];	n3z = m[2][3] + m[2][2];	d3 = m[3][3] + m[3][2];
			break;
		case 3:
			n1x = m[0][3] + m[0][0];	n1y = m[1][3] + m[1][0];	n1z = m[2][3] + m[2][0];	d1 = m[3][3] + m[3][0];
			n2x = m[0][3] - m[0][1];	n2y = m[1][3] - m[1][1];	n1z = m[2][3] - m[2][1];	d2 = m[3][3] - m[3][1];
			n3x = m[0][3] + m[0][2];	n3y = m[1][3] + m[1][2];	n3z = m[2][3] + m[2][2];	d3 = m[3][3] + m[3][2];
			break;
		case 4:
			n1x = m[0][3] - m[0][0];	n1y = m[1][3] - m[1][0];	n1z = m[2][3] - m[2][0];	d1 = m[3][3] - m[3][0];
			n2x = m[0][3] + m[0][1];	n2y = m[1][3] + m[1][1];	n1z = m[2][3] + m[2][1];	d2 = m[3][3] + m[3][1];
			n3x = m[0][3] - m[0][2];	n3y = m[1][3] - m[1][2];	n3z = m[2][3] - m[2][2];	d3 = m[3][3] - m[3][2];
			break;
		case 5:
			n1x = m[0][3] + m[0][0];	n1y = m[1][3] + m[1][0];	n1z = m[2][3] + m[2][0];	d1 = m[3][3] + m[3][0];
			n2x = m[0][3] + m[0][1];	n2y = m[1][3] + m[1][1];	n1z = m[2][3] + m[2][1];	d2 = m[3][3] + m[3][1];
			n3x = m[0][3] - m[0][2];	n3y = m[1][3] - m[1][2];	n3z = m[2][3] - m[2][2];	d3 = m[3][3] - m[3][2];
			break;
		case 6:
			n1x = m[0][3] + m[0][0];	n1y = m[1][3] + m[1][0];	n1z = m[2][3] + m[2][0];	d1 = m[3][3] + m[3][0];
			n2x = m[0][3] - m[0][1];	n2y = m[1][3] - m[1][1];	n1z = m[2][3] - m[2][1];	d2 = m[3][3] - m[3][1];
			n3x = m[0][3] - m[0][2];	n3y = m[1][3] - m[1][2];	n3z = m[2][3] - m[2][2];	d3 = m[3][3] - m[3][2];
			break;
		case 7:
			n1x = m[0][3] - m[0][0];	n1y = m[1][3] - m[1][0];	n1z = m[2][3] - m[2][0];	d1 = m[3][3] - m[3][0];
			n2x = m[0][3] - m[0][1];	n2y = m[1][3] - m[1][1];	n1z = m[2][3] - m[2][1];	d2 = m[3][3] - m[3][1];
			n3x = m[0][3] - m[0][2];	n3y = m[1][3] - m[1][2];	n3z = m[2][3] - m[2][2];	d3 = m[3][3] - m[3][2];
			break;
		}

		float 
			c23x = 0.0f, c23y = 0.0f, c23z = 0.0f,
			c31x = 0.0f, c31y = 0.0f, c31z = 0.0f,
			c12x = 0.0f, c12y = 0.0f, c12z = 0.0f;

		c23x = n2y * n3z - n2z * n3y;
		c23y = n2z * n3x - n2x * n3z;
		c23z = n2x * n3y - n2y * n3x;

		c31x = n3y * n1z - n3z * n1y;
		c31y = n3z * n1x - n3x * n1z;
		c31z = n3x * n1y - n3y * n1x;

		c12x = n1y * n2z - n1z * n2y;
		c12y = n1z * n2x - n1x * n2z;
		c12z = n1x * n2y - n1y * n2x;

		float inverse = 1.0f / (n1x * c23x + n1y * c23y + n1z * c23z);

		point.x = (-c23x * d1 - c31x * d2 - c12x * d3) * inverse;
		point.y = (-c23y * d1 - c31y * d2 - c12y * d3) * inverse;
		point.z = (-c23z * d1 - c31z * d2 * c12z * d3) * inverse;

		return point;
	}

}
