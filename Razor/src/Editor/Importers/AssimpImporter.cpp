#include "rzpch.h"
#include "AssimpImporter.h"
#include "Razor/Geometry/StaticMesh.h"
#include "Razor/Core/Transform.h"
#include "Razor/Core/Utils.h"
#include "Razor/Maths/Maths.h"
#include "Razor/Materials/Material.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/Materials/Presets/PbrMaterial.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Editor/Components/AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Rendering/ForwardRenderer.h"

namespace Razor 
{

	AssimpImporter::AssimpImporter() : 
		Assimp::ProgressHandler(),
		percent(0.0f) 
	{
	}

	AssimpImporter::~AssimpImporter()
	{
		delete importer;
	}

	bool AssimpImporter::Update(float percentage)
	{
		percent = percentage;
		return true;
	}
	
	bool AssimpImporter::importMesh(const std::string& filename)
	{
		importer = new Assimp::Importer();
		importer->SetProgressHandler((Assimp::ProgressHandler*)this);

		scene = importer->ReadFile(filename,
			aiProcess_CalcTangentSpace |
			//aiProcess_GenSmoothNormals |
			aiProcess_Triangulate
			//aiProcess_JoinIdenticalVertices |
			//aiProcess_PreTransformVertices
			//aiProcess_FlipWindingOrder
			//aiProcess_RemoveRedundantMaterials |
			//aiProcess_OptimizeMeshes |
			//aiProcess_OptimizeGraph |
			//aiProcess_ImproveCacheLocality
		);

		if (!scene) {
			Log::warn("Can't load mesh file: %s", filename.c_str());
			Log::warn(importer->GetErrorString());
			return false;
		}
	
		if (scene->HasMeshes())
		{
			for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
				std::shared_ptr<StaticMesh> mesh = processMesh(scene->mMeshes[i]);
				meshes.resize(i);
				meshes.push_back(mesh);
			}
		}
		else
		{
			Log::warn("Error: No meshes found");
			return false;
		}

		if (scene->mRootNode != NULL)
		{
			rootNode = std::make_shared<Node>();
			auto parts = Utils::splitString(filename, ".");
			auto pathname = parts[0];
			auto str = pathname.substr(pathname.find_last_of("/") + 1);
			rootNode->name = str;
			processNode(scene->mRootNode, nullptr, rootNode);
			//resetRootNode();
		}
		else
		{
			Log::warn("Error loading model");
			return false;
		}

		importer->FreeScene();

		return true;
	}

	void AssimpImporter::processNode(aiNode* node, std::shared_ptr<Node> parentNode, std::shared_ptr<Node> newNode)
	{
		const char* name = node->mName.length != 0 ? node->mName.C_Str() : "child_node";

		glm::mat4 mat = parentNode != nullptr 
			? parentNode->transform.getMatrix() * glm::mat4(node->mTransformation[0][0])
			: glm::mat4(node->mTransformation[0][0]);

		newNode->name = name;
		newNode->transform = Transform();
		newNode->transform.setMatrix(mat);
		newNode->parent = parentNode;
		newNode->meshes.resize(node->mNumMeshes);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {

			std::shared_ptr<StaticMesh> m = meshes[node->mMeshes[i]];
			newNode->meshes[i] = m;
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{ 
			std::shared_ptr<Node> n = std::make_shared<Node>();
			n->name = std::string(node->mName.C_Str());
			newNode->nodes.push_back(n);
			this->processNode(node->mChildren[i], parentNode, newNode->nodes[i]);
		}

		/*std::sort(newNode->nodes.begin(), newNode->nodes.end(),
		[](const Node& a, const Node& b) -> bool
		{
			return a.name > b.name;
		});*/
	}

	std::shared_ptr<StaticMesh> AssimpImporter::processMesh(aiMesh* object)
	{
		std::string name = object->mName.length != 0 ? std::string(object->mName.C_Str()) : "child_node";
		std::shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>();
		mesh->setName(name);
		mesh->setVertexCount(object->mNumVertices);
		AABB box;
		std::shared_ptr<PbrMaterial> material = std::make_shared<PbrMaterial>();
		std::string textures_path = "./data/";

		if (object->mMaterialIndex >= 0)
		{
			aiMaterial* mat = getMaterial(object->mMaterialIndex);

			// Diffuse
			aiString diffusePath;
			if (AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath))
			{
				if (strlen(diffusePath.C_Str()) > 0)
				{
					std::string diffuse_filename = textures_path + extractTextureFilename(diffusePath);

					Texture* diffuseTexture;
					if (!AssetsManager::texturesManager->hasTexture(diffuse_filename))
					{
						diffuseTexture = new Texture(diffuse_filename, true);
						AssetsManager::texturesManager->addTexture(diffuse_filename, diffuseTexture);
					}
					else
						diffuseTexture = AssetsManager::texturesManager->getTexture(diffuse_filename);

					material->setTextureMap(Material::TextureType::Diffuse, diffuseTexture->getId());
				}
			}
			
			// Specular
			aiString specularPath;
			if (AI_SUCCESS == mat->GetTexture(aiTextureType_SPECULAR, 0, &specularPath))
			{
				if (strlen(specularPath.C_Str()) > 0)
				{
					std::string specular_filename = textures_path + extractTextureFilename(specularPath);
					Texture* specularTexture;
					if (!AssetsManager::texturesManager->hasTexture(specular_filename))
					{
						specularTexture = new Texture(textures_path + extractTextureFilename(specularPath), true);
						AssetsManager::texturesManager->addTexture(specular_filename, specularTexture);
					}
					else
						specularTexture = AssetsManager::texturesManager->getTexture(specular_filename);

					material->setTextureMap(Material::TextureType::Specular, specularTexture->getId());
				}
			}

			// Normal
			aiString normalPath;
			if (AI_SUCCESS == mat->GetTexture(aiTextureType_DISPLACEMENT, 0, &normalPath))
			{
				if (strlen(normalPath.C_Str()) > 0)
				{
					std::string normal_filename = textures_path + extractTextureFilename(normalPath);
					Texture* normalTexture;
					if (!AssetsManager::texturesManager->hasTexture(normal_filename))
					{
						normalTexture = new Texture(textures_path + extractTextureFilename(normalPath), true);
						AssetsManager::texturesManager->addTexture(normal_filename, normalTexture);
					}
					else
						normalTexture = AssetsManager::texturesManager->getTexture(normal_filename);

					material->setTextureMap(Material::TextureType::Normal, normalTexture->getId());
				}
			}

			// Parameters
			aiString materialName;
			if(AI_SUCCESS == mat->Get(AI_MATKEY_NAME, materialName))
				material->setName(materialName.C_Str());

			//float shininess;
			//if(AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, shininess))
			//	material->setShininess(shininess);

			//float shininess_strength;
			//if(AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength))
			//	material->setShininessStrength(shininess_strength);

			////aiColor3D diffuse;
			////if(AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			////	material->setDiffuseColor(glm::vec3(diffuse.r, diffuse.g, diffuse.b));

			//aiColor3D specular;
			//if(AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, specular))
			//	material->setSpecularColor(glm::vec3(specular.r, specular.g, specular.b));

			////aiColor3D ambient;
			////if(AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
			////	material->setAmbientColor(glm::vec3(ambient.r, ambient.g, ambient.b));

			//aiColor3D emissive;
			//if(AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive))
			//	material->setEmissiveColor(glm::vec3(emissive.r, emissive.g, emissive.b));

			//float alpha;
			//if(AI_SUCCESS == mat->Get(AI_MATKEY_OPACITY, alpha))
			//	material->setAlpha(alpha);
		}

		mesh->setMaterial(material);

		if (object->HasFaces())
		{
			for (unsigned int i = 0; i < object->mNumFaces; i++)
			{
				mesh->getIndices().push_back(object->mFaces[i].mIndices[0]);
				mesh->getIndices().push_back(object->mFaces[i].mIndices[1]);
				mesh->getIndices().push_back(object->mFaces[i].mIndices[2]);

				//std::cout << "ind: " << object->mFaces[i].mIndices[0] << ", " << object->mFaces[i].mIndices[1] << ", " << object->mFaces[i].mIndices[2] << std::endl;
			}
		}

		for (unsigned int i = 0; i < object->mNumVertices; i++)
		{
			if (object->HasPositions())
			{
				mesh->getVertices().push_back(object->mVertices[i].x);
				mesh->getVertices().push_back(object->mVertices[i].y);
				mesh->getVertices().push_back(object->mVertices[i].z);

				//std::cout << "vert: " << object->mVertices[i].x << ", " << object->mVertices[i].y << ", " << object->mVertices[i].z << std::endl;
			}

			if (object->HasNormals()) 
			{
				mesh->getNormals().push_back(object->mNormals[i].x);
				mesh->getNormals().push_back(object->mNormals[i].y);
				mesh->getNormals().push_back(object->mNormals[i].z);

				//std::cout << "norm: " << object->mNormals[i].x << ", " << object->mNormals[i].y << ", " << object->mNormals[i].z << std::endl;
			}

			if (object->HasTextureCoords(0))
			{
				mesh->getUvs().push_back(object->mTextureCoords[0][i].x);
				mesh->getUvs().push_back(object->mTextureCoords[0][i].y);

				//std::cout << "uvs: " << object->mTextureCoords[0][i].x << ", " << object->mTextureCoords[0][i].y << std::endl;
			}
			
			if (object->HasTangentsAndBitangents()) 
			{
				mesh->getTangents().push_back(object->mTangents[i].x);
				mesh->getTangents().push_back(object->mTangents[i].y);
				mesh->getTangents().push_back(object->mTangents[i].z);

				//std::cout << "tan: " << object->mTangents[i].x << ", " << object->mTangents[i].y << ", " << object->mTangents[i].z << std::endl;
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

	aiMaterial* AssimpImporter::getMaterial(unsigned int index)
	{
		return scene->mMaterials[index];
	}

	std::string AssimpImporter::extractTextureFilename(const aiString& path)
	{
		std::set<char> delims{ '\\' };
		std::vector<std::string> str = Utils::splitPath(path.C_Str(), delims);

		return str.back();
	}

}
