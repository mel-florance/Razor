#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Scene/Node.h"
#include "Razor/Types/Variant.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ProgressHandler.hpp>

namespace Razor 
{

	class AssimpImporter : public Assimp::ProgressHandler
	{
	public:
		AssimpImporter();
		virtual ~AssimpImporter();

		inline Node* getNodeData() { return rootNode.get(); }

		bool importMesh(const std::string& filename);
		void processNode(aiNode* node, std::shared_ptr<Node> parentNode, std::shared_ptr<Node> newNode);
		std::shared_ptr<StaticMesh> processMesh(aiMesh* object);
		aiMaterial* getMaterial(unsigned int index);

		std::string extractTextureFilename(const aiString& path);

		inline void resetRootNode() 
		{
			for (auto mesh : meshes)
				mesh.reset();

			meshes.clear();
			rootNode.reset();
		}

		bool Update(float percentage = -1.f) override;
		float percent;

		std::shared_ptr<Node> rootNode;
		std::vector<std::shared_ptr<StaticMesh>> meshes;

	private:
		const aiScene* scene;
		Assimp::Importer* importer;
	};

}
