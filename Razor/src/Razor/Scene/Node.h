#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Core/Transform.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Cameras/Camera.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Landscape/Landscape.h"

namespace Razor 
{

	class Node
	{
	public:
		typedef std::vector<std::shared_ptr<Node>> List;

		Node();
		Node(std::shared_ptr<Node> n)
		{
			name = n->name;
			nodes = n->nodes;
			meshes = n->meshes;
			lights = n->lights;
			cameras = n->cameras;
			landscapes = n->landscapes;
			transform = n->transform;
			parent = n->parent;
			id = n->id;
			isInstance = n->isInstance;
		}
		Node(Node* n)
		{
			name = n->name;
			nodes = n->nodes;
			meshes = n->meshes;
			lights = n->lights;
			cameras = n->cameras;
			landscapes = n->landscapes;
			transform = n->transform;
			parent = n->parent;
			id = n->id;
			isInstance = n->isInstance;
		}
		Node(Node& n)
		{
			name = n.name;
			nodes = n.nodes;
			meshes = n.meshes;
			lights = n.lights;
			cameras = n.cameras;
			landscapes = n.landscapes;
			transform = n.transform;
			parent = n.parent;
			id = n.id;
			isInstance = n.isInstance;
		}
		~Node();

		void setupMeshBuffers(std::shared_ptr<Node> node);

		std::string name;
		std::vector<std::shared_ptr<Node>> nodes;
		std::vector<std::shared_ptr<StaticMesh>> meshes;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<Landscape>> landscapes;
		Transform transform;
		std::shared_ptr<Node> parent;
		unsigned int id;
		bool isInstance;
		bool opened;
		bool active;
	};

}
