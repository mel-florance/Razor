#pragma once

#include "Razor/Core.h"
#include "Razor/Node.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Razor {

	class RZ_API AssimpImporter
	{
	public:
		AssimpImporter();

		inline Node* getNodeData() { return rootNode; }

		bool importMesh(const std::string& filename);
		void processNode(const aiScene* scene, aiNode* node, Node* parentNode, Node* newNode);
		std::shared_ptr<Mesh> processMesh(aiMesh* object);

	private:
		std::unique_ptr<Assimp::Importer> importer;

		Node* rootNode;
		std::vector<std::shared_ptr<Mesh>> meshes;

		std::vector<unsigned int> m_indices;
		std::vector<float> m_vertices;
		std::vector<float> m_uvs;
		std::vector<float> m_normals;
		std::vector<float> m_tangents;
	};

}