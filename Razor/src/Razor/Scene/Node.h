#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Core/Transform.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Lighting/Light.h"

namespace Razor 
{

	class RAZOR_API Node
	{
	public:
		Node();
		Node(std::shared_ptr<Node> n)
		{
			this->name = n->name;
			this->nodes = n->nodes;
			this->meshes = n->meshes;
			this->transform = n->transform;
			this->parent = n->parent;
			this->id = n->id;
		}
		~Node();

		std::string name;
		std::vector<std::shared_ptr<Node>> nodes;
		std::vector<std::shared_ptr<StaticMesh>> meshes;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Camera>> cameras;
		Transform transform;
		std::shared_ptr<Node> parent;
		unsigned int id;
		bool isInstance;
	};

}
