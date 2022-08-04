#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <unordered_map>
#include <vector>
#include <string>

namespace Snow
{
	class Model  // #TODO: Move myModels map to asset class that handles loading and caching
	{
	public:
		bool Initialize(const char* aFilepath);
		void Initialize(Mesh aMesh);

	private:
		friend class Renderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		bool LoadModel(const char* aFilepath);
		bool ProcessNode(aiNode* aNode, const aiScene* aScene);
		Mesh ProcessMesh(aiMesh* aMesh);

		std::string myFilePath = "";
		std::vector<Mesh> myMeshes;

		inline static std::unordered_map<std::string, Ref<Model>> myModels;
	};
}