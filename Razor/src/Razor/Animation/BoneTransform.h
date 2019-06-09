#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Razor
{

	class BoneTransform
	{
	public:
		BoneTransform(const glm::vec3& position, const glm::quat& rotation);
		~BoneTransform();

		inline glm::mat4 BoneTransform::getLocaltransform()
		{
			glm::mat4 matrix;
			glm::translate(matrix, position);

			return matrix * glm::toMat4(rotation);
		}

		inline static BoneTransform* BoneTransform::interpolate(BoneTransform* a, BoneTransform* b, float time)
		{
			glm::vec3 pos = Utils::lerp(a->position, b->position, time);
			glm::quat rot = glm::slerp(a->rotation, b->rotation, time);

			return new BoneTransform(pos, rot);
		}

	private:
		glm::vec3 position;
		glm::quat rotation;
	};

}
