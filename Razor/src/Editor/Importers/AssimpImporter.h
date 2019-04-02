#pragma once

#include "Razor/Core.h"
#include "Razor/Node.h"
#include "Razor/Variant.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ProgressHandler.hpp>

namespace Razor {

	class AssimpImporter : public Assimp::ProgressHandler
	{
	public:
		AssimpImporter();

		inline Node* getNodeData() { return rootNode; }

		bool importMesh(const std::string& filename);
		void processNode(const aiScene* scene, aiNode* node, Node* parentNode, Node* newNode);
		std::shared_ptr<Mesh> processMesh(aiMesh* object);

		bool Update(float percentage = -1.f);
		float percent;

		Node* rootNode;
		std::vector<std::shared_ptr<Mesh>> meshes;

	private:
		std::vector<unsigned int> m_indices;
		std::vector<float> m_vertices;
		std::vector<float> m_uvs;
		std::vector<float> m_normals;
		std::vector<float> m_tangents;
	};

}