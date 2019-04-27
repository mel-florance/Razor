#include "rzpch.h"
#include "AssimpImporter.h"
#include "Razor/Geometry/Mesh.h"
#include "Razor/Core/Transform.h"
#include "Razor/Core/Utils.h"
#include "Razor/Maths/Maths.h"

namespace Razor {

	AssimpImporter::AssimpImporter() : ProgressHandler()
	{
		percent = 0.0f;
	}

	bool AssimpImporter::Update(float percentage)
	{
		percent = percentage;
		return true;
	}
	
	bool AssimpImporter::importMesh(const std::string& filename)
	{
		Assimp::Importer importer;
		importer.SetProgressHandler((Assimp::ProgressHandler*)this);

		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_Triangulate
		);

		if (!scene)
		{
			RZ_WARN(importer.GetErrorString());
			return false;
		}	
	
		if (scene->HasMeshes())
		{
			for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
				Mesh* mesh = processMesh(scene->mMeshes[i]);
				meshes.resize(i);
				meshes.push_back(mesh);
			}
		}
		else
		{
			RZ_WARN("Error: No meshes found");
			return false;
		}

		if (scene->mRootNode != NULL)
		{
			rootNode = new Node();
			processNode(scene, scene->mRootNode, 0, rootNode);
			//this->rootNode.reset(rootNode);
		}
		else
		{
			RZ_WARN("Error loading model");
			return false;
		}

		return true;
	}

	void AssimpImporter::processNode(const aiScene* scene, aiNode* node, Node* parentNode, Node* newNode)
	{
		const char* name = node->mName.length != 0 ? node->mName.C_Str() : "child_node";

		newNode->name = name;
		//newNode->id = QRandomGenerator::global()->generate();
		
		newNode->transform = Transform();
		newNode->transform.setMatrix(glm::mat4(node->mTransformation[0][0]));
		newNode->meshes.resize(node->mNumMeshes);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
			newNode->meshes[i] = meshes[node->mMeshes[i]];

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			newNode->nodes.push_back(new Node());
			this->processNode(scene, node->mChildren[i], parentNode, newNode->nodes[i]);
		}
	}

	Mesh* AssimpImporter::processMesh(aiMesh* object)
	{
		const char* name = object->mName.length != 0 ? object->mName.C_Str() : "";
		Mesh* mesh = new Mesh();
		mesh->setName(name);
		BoundingBox box;

		if (object->HasFaces())
		{
			for (unsigned int i = 0; i < object->mNumFaces; i++)
			{
				mesh->getIndices().push_back(object->mFaces[i].mIndices[0]);
				mesh->getIndices().push_back(object->mFaces[i].mIndices[1]);
				mesh->getIndices().push_back(object->mFaces[i].mIndices[2]);
			}
		}

		for (unsigned int i = 0; i < object->mNumVertices; i++)
		{
			if (object->HasPositions()) {
				mesh->getVertices().push_back(object->mVertices[i].x);
				mesh->getVertices().push_back(object->mVertices[i].y);
				mesh->getVertices().push_back(object->mVertices[i].z);
			}

			if (object->HasNormals()) {
				mesh->getVertices().push_back(object->mNormals[i].x);
				mesh->getVertices().push_back(object->mNormals[i].y);
				mesh->getVertices().push_back(object->mNormals[i].z);
			}

			if (object->HasTextureCoords(0))
			{
				mesh->getVertices().push_back(object->mTextureCoords[0][i].x);
				mesh->getVertices().push_back(object->mTextureCoords[0][i].y);
			}
			
			if (object->HasTangentsAndBitangents()) {
				mesh->getVertices().push_back(object->mTangents[i].x);
				mesh->getVertices().push_back(object->mTangents[i].y);
				mesh->getVertices().push_back(object->mTangents[i].z);
			}

			if (object->mVertices[i].x < box.min_x) box.min_x = object->mVertices[i].x;
			if (object->mVertices[i].x > box.max_x) box.max_x = object->mVertices[i].x;
			if (object->mVertices[i].y < box.min_y) box.min_y = object->mVertices[i].y;
			if (object->mVertices[i].y < box.max_y) box.max_y = object->mVertices[i].y;
			if (object->mVertices[i].z < box.min_z) box.min_z = object->mVertices[i].z;
			if (object->mVertices[i].z < box.max_z) box.max_z = object->mVertices[i].z;
		}

		mesh->setBoundingBox(box);

		return mesh;
	}

}
