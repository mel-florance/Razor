#include "rzpch.h"

#include "BoneTransform.h"
#include "Bone.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Razor/Geometry/SkeletalMesh.h"

namespace Razor
{
	AnimationManager::AnimationManager(SkeletalMesh* mesh) :
		mesh(mesh),
		current_animation(nullptr),
		delta(0.0f),
		time(0.0f)

	{
	}

	AnimationManager::~AnimationManager()
	{
	}

	void AnimationManager::update(float dt)
	{
		delta = dt;

		if (current_animation == nullptr)
			return;

		tick();

		std::map<std::string, glm::mat4> current_pose = calcCurrentAnimationPose();
		applyPoseToBone(current_pose, mesh->getRootBone(), glm::mat4(1.0f));
	}

	void AnimationManager::tick()
	{
		time += delta;

		if (time > current_animation->getLength())
			time = fmod(time, current_animation->getLength());
	}

	void AnimationManager::playAnimation(Animation* animation)
	{
		time = 0.0f;
		current_animation = animation;
	}

	std::map<std::string, glm::mat4> AnimationManager::calcCurrentAnimationPose()
	{
		std::vector<Keyframe> frames = getPreviousAndNextFrames();
		float progress = calculateProgress(frames[0], frames[1]);

		return interpolatePoses(frames[0], frames[1], progress);
	}

	void AnimationManager::applyPoseToBone(const std::map<std::string, glm::mat4>& current_pose, std::shared_ptr<Bone> bone, glm::mat4 parent_transform)
	{
		glm::mat4 current_local_transform = current_pose.at(bone->getName());
		glm::mat4 current_transform = parent_transform * current_local_transform;

		for (auto child : bone->getChildren())
			applyPoseToBone(current_pose, child, current_transform);

		current_transform *= bone->getInverseTransform();
		bone->setAnimatedTransform(current_transform);
	}

	std::vector<Keyframe> AnimationManager::getPreviousAndNextFrames()
	{
		std::vector<Keyframe> frames = current_animation->getKeyframes();

		Keyframe previous = frames[0];
		Keyframe next = frames[0];

		for (int i = 1; i < frames.size(); i++)
		{
			next = frames[i];

			if (next.getTimestamp() > time)
				break;

			previous = frames[i];
		}

		std::vector<Keyframe> f = { previous, next };

		return f;
	}

	float AnimationManager::calculateProgress(Keyframe previous, Keyframe next)
	{
		float total = next.getTimestamp() - previous.getTimestamp();
		float current = time - previous.getTimestamp();

		return current / total;
	}

	std::map<std::string, glm::mat4> AnimationManager::interpolatePoses(Keyframe previous, Keyframe next, float progress)
	{
		std::map<std::string, glm::mat4> currentPose;
		std::map<std::string, BoneTransform*>::iterator it;
		std::map<std::string, BoneTransform*> keyframes = previous.getPose();

		for (it = keyframes.begin(); it != keyframes.end(); it++)
		{
			BoneTransform* previousTransform = keyframes.at(it->first);
			BoneTransform* nextTransform = next.getPose().at(it->first);
			BoneTransform* currentTransform = BoneTransform::interpolate(previousTransform, nextTransform, progress);

			currentPose[it->first] = currentTransform->getLocaltransform();
		}

		return currentPose;
	}

}