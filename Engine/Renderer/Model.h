#pragma once
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model
{
public:
	bool Initialize(const char* aFilepath);
	void Initialize(Mesh aMesh);
	void SetTexture(ID3D11ShaderResourceView* aTexture);

private:
	friend class Renderer;
	bool LoadModel(const char* aFilepath);
	bool ProcessNode(aiNode* aNode, const aiScene* aScene);
	Mesh ProcessMesh(aiMesh* aMesh);

	std::vector<Mesh> myMeshes;
};