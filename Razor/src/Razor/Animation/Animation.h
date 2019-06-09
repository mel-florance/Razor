#pragma once

#include "Keyframe.h"

namespace Razor
{
	class Animation
	{
	public:
		Animation();
		~Animation();

		inline float& getLength() { return length; }
		inline std::vector<Keyframe>& getKeyframes() { return keyframes; }

		inline void setLength(float length) { this->length = length; }
		inline void setKeyframes(const std::vector<Keyframe>& keyframes) { this->keyframes = keyframes; }

	private:
		float length;
		std::vector<Keyframe> keyframes;
	};

}
