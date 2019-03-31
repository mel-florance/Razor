#include "rzpch.h"
#include "AssimpImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void AssimpImporter::load(const std::string& filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
		| aiProcess_Triangulate
	);

	return;
}

