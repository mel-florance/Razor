#include "rzpch.h"
#include "Keyframe.h"
#include "BoneTransform.h"

namespace Razor
{
	Keyframe::Keyframe(float timestamp, std::map<std::string, BoneTransform*> keyframes) :
		timestamp(timestamp),
		pose(keyframes)
	{
	}

	Keyframe::~Keyframe()
	{
	}

}