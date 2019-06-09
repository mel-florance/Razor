#pragma once

namespace Razor
{
	class Bone;

	class SkeletalMesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();

		inline std::shared_ptr<Bone> getRootBone() { return root_bone; }

	private:
		std::shared_ptr<Bone> root_bone;
	};

}
