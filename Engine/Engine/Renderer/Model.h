#pragma once
#include "Mesh.h"
#include "Engine/Core/Base.h"
#include "Engine/Debug/Log.h"

#include <unordered_map>
#include <vector>
#include <string>

struct aiScene;
struct aiNode;
struct aiMesh;

namespace Assimp
{
	class Importer;
}

namespace Snow
{
	class Model  // #TODO: Move myModels map to asset class that handles loading and caching
	{
	public:
		Model() = default;
		~Model() = default;
		bool LoadModel(const char* aFilepath);

	private:
		friend class Renderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		bool ProcessNode(aiNode* aNode, const aiScene* aScene);
		Mesh ProcessMesh(aiMesh* aMesh);
		void LoadBones(std::vector<Bone>& aBones, std::vector<Vertex>& vertices, aiMesh* aMesh);

		Ref<Assimp::Importer> myImporter;

		std::string myFilePath = "";
		std::vector<Mesh> myMeshes;

		inline static std::unordered_map<std::string, Ref<Model>> myModels;
	};
}