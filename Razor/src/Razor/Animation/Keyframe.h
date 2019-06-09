#pragma once

namespace Razor
{
	class BoneTransform;

	class Keyframe
	{
	public:
		Keyframe(float timestamp, std::map<std::string, BoneTransform*> keyframes);
		~Keyframe();

		inline float& getTimestamp() { return timestamp; }
		inline std::map<std::string, BoneTransform*> getPose() { return pose; }

		inline void setTimestamp(float value) { timestamp = value; }
		inline void setPose(std::map<std::string, BoneTransform*> keyframes) { this->pose = keyframes; }

	private:
		float timestamp;
		std::map<std::string, BoneTransform*> pose;
	};

}
