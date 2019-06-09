#pragma once

namespace Razor
{

	class Bone
	{
	public:
		Bone(const std::string& name, const glm::mat4& local_transform);
		~Bone();

		inline void addChild(std::shared_ptr<Bone> bone) { children.push_back(bone); }
		void calcInverseTransform(const glm::mat4& parent);

		inline unsigned int& getId() { return id; }
		inline std::string& getName() { return name; }
		inline std::vector<std::shared_ptr<Bone>>& getChildren() { return children; }
		inline glm::mat4& getAnimatedTransform() { return animated_transform; }
		inline glm::mat4& getLocalTransform() { return local_transform; }
		inline glm::mat4& getInverseTransform() { return inverse_transform; }

		inline void setId(unsigned int id) { this->id = id; }
		inline void setName(const std::string& name) { this->name = name; }
		inline void setChildren(const std::vector<std::shared_ptr<Bone>>& children) { this->children = children; }
		inline void setAnimatedTransform(const glm::mat4& transform) { this->animated_transform = transform; }
		inline void setLocalTransform(const glm::mat4& transform) { this->local_transform = transform; }
		inline void setInverseTransform(const glm::mat4& transform) { this->inverse_transform = transform; }

	private:
		unsigned int id;
		std::string name;
		std::vector<std::shared_ptr<Bone>> children;
		glm::mat4 animated_transform;
		glm::mat4 local_transform;
		glm::mat4 inverse_transform;
	};

}
