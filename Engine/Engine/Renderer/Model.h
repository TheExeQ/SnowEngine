#pragma once
#include "Mesh.h"
#include "Engine/Core/Base.h"
#include "Engine/Debug/Log.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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
	class Animation;

	struct Bone
	{
		glm::mat4 offsetTransform;
		glm::mat4 finalTransform;
	};

	class Model  // #TODO: Move myModels map to asset class that handles loading and caching
	{
	public:
		Model() = default;
		~Model() = default;
		bool LoadModel(const char* aFilepath);
		void SetAnimation(const Animation& aAnimation);

	private:
		friend class SceneRenderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		bool ProcessNode(aiNode* aNode, const aiScene* aScene);
		Mesh ProcessMesh(aiMesh* aMesh);
		void LoadBones(std::vector<Vertex>& vertices, aiMesh* aMesh);

		void UpdateBoneTransform(float aTime);
		void ReadNodeHierarchy(float aAnimationTime, const aiNode* aNode, const glm::mat4& aParentTransform);

		Ref<Assimp::Importer> myImporter;
		const aiScene* myScene;
		const Animation* myCurrentAnimation;

		std::vector<Bone> myBones;
		std::unordered_map<std::string, uint32_t> myBoneMapping;
		std::vector<glm::mat4> myBoneTransforms;
		glm::mat4 myInverseTransform = glm::mat4(1.f);

		std::string myFilePath = "";
		std::vector<Mesh> myMeshes;

		inline static std::unordered_map<std::string, Ref<Model>> myModels;
	};
}