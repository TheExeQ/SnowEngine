#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <string>

class Model
{
public:
	bool Initialize(const char* aFilepath);
	void Initialize(Mesh aMesh);

private:
	friend class Renderer;
	friend class SceneHierarchyPanel;
	bool LoadModel(const char* aFilepath);
	bool ProcessNode(aiNode* aNode, const aiScene* aScene);
	Mesh ProcessMesh(aiMesh* aMesh);

	std::string myFilePath = "";
	std::vector<Mesh> myMeshes;
};