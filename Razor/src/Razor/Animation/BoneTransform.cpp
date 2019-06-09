#include "rzpch.h"
#include "BoneTransform.h"

namespace Razor
{
	BoneTransform::BoneTransform(const glm::vec3& position, const glm::quat& rotation) :
		position(position),
		rotation(rotation)
	{
	}

	BoneTransform::~BoneTransform()
	{
	}

}
