#include "rzpch.h"
#include "AssimpImporter.h"
#include "Razor/Mesh.h"

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
			RZ_CORE_WARN(importer.GetErrorString());
			return false;
		}	
	
		if (scene->HasMeshes())
		{
			for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
				std::shared_ptr<Mesh> mesh = processMesh(scene->mMeshes[i]);
				meshes.resize(i);
				meshes.push_back(mesh);
			}
		}
		else
		{
			RZ_CORE_WARN("Error: No meshes found");
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
			RZ_CORE_WARN("Error loading model");
			return false;
		}

		return true;
	}

	void AssimpImporter::processNode(const aiScene* scene, aiNode* node, Node* parentNode, Node* newNode)
	{
		const char* name = node->mName.length != 0 ? node->mName.C_Str() : "child_node";

		newNode->name = name;
		//newNode->id = QRandomGenerator::global()->generate();
	/*	newNode->transform = new Transform();*/
		//newNode.transform->setWorld(QMatrix4x4(node->mTransformation[0]));
		newNode->meshes.resize(node->mNumMeshes);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			newNode->meshes[i] = meshes[node->mMeshes[i]].get();
		}	

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			newNode->nodes.push_back(new Node());
			this->processNode(scene, node->mChildren[i], parentNode, newNode->nodes[i]);
		}
	}

	std::shared_ptr<Mesh> AssimpImporter::processMesh(aiMesh* object)
	{
		const char* name = object->mName.length != 0 ? object->mName.C_Str() : "";
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->setName(name);

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


			if (object->HasTextureCoords(0))
			{
				mesh->getUvs().push_back(object->mTextureCoords[0][i].x);
				mesh->getUvs().push_back(object->mTextureCoords[0][i].y);
			}

			if (object->HasNormals()) {
				mesh->getNormals().push_back(object->mNormals[i].x);
				mesh->getNormals().push_back(object->mNormals[i].y);
				mesh->getNormals().push_back(object->mNormals[i].z);
			}

			if (object->HasTangentsAndBitangents()) {
				mesh->getTangents().push_back(object->mTangents[i].x);
				mesh->getTangents().push_back(object->mTangents[i].y);
				mesh->getTangents().push_back(object->mTangents[i].z);
			}
		}

		m_indices = mesh->getIndices();
		m_vertices = mesh->getVertices();
		m_normals = mesh->getNormals();
		m_uvs = mesh->getUvs();
		m_tangents = mesh->getTangents();

		return mesh;
	}

}


