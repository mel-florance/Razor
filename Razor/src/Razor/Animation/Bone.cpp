#include "rzpch.h"
#include "Bone.h"

namespace Razor
{

	Bone::Bone(const std::string& name, const glm::mat4& local_transform) :
		id(0),
		name(name),
		children({}),
		local_transform(local_transform),
		animated_transform(glm::mat4(1.0f)),
		inverse_transform(glm::mat4(1.0f))
	{
	}

	Bone::~Bone()
	{
	}

	void Bone::calcInverseTransform(const glm::mat4& parent)
	{
		glm::mat4 transform = parent * local_transform;
		inverse_transform = glm::inverse(transform);

		for (auto bone : children)
			bone->calcInverseTransform(transform);
	}

}
