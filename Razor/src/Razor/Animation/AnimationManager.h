#pragma once

namespace Razor
{
	class SkeletalMesh;
	class Animation;
	class Bone;
	class Keyframe;

	class AnimationManager
	{
	public:
		AnimationManager(SkeletalMesh* mesh);
		~AnimationManager();

		void update(float dt);
		void tick();
		void playAnimation(Animation* animation);
		std::map<std::string, glm::mat4> calcCurrentAnimationPose();
		void applyPoseToBone(const std::map<std::string, glm::mat4>& current_pose, std::shared_ptr<Bone> bone, glm::mat4 parent_transform);
		std::vector<Keyframe> getPreviousAndNextFrames();
		float calculateProgress(Keyframe previous, Keyframe next);
		std::map<std::string, glm::mat4> interpolatePoses(Keyframe previous, Keyframe next, float progress);

	private:
		SkeletalMesh* mesh;
		Animation* current_animation;
		float delta;
		float time;
	};

}
