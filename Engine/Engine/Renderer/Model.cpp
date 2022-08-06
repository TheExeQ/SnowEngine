#include "Model.h"
#include "Engine/Math/Math.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Snow
{
	bool Model::LoadModel(const char* aFilepath)
	{
		auto it = myModels.find(std::string(aFilepath));
		if (it != myModels.end())
		{
			myFilePath = it->second->myFilePath;
			myMeshes = it->second->myMeshes;
			CORE_LOG_INFO("Model reused.");
			return true;
		}

		myImporter = CreateRef<Assimp::Importer>();

		const aiScene* scene = myImporter->ReadFile(aFilepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (!scene)
		{
			CORE_LOG_WARN(std::string("Failed to load model: ") + std::string(aFilepath));
			return false;
		}

		myMeshes.clear();

		ProcessNode(scene->mRootNode, scene);
		myFilePath = aFilepath;
		myModels[std::string(aFilepath)] = CreateRef<Model>(*this);
		return true;
	}

	bool Model::ProcessNode(aiNode* aNode, const aiScene* aScene)
	{
		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
			myMeshes.push_back(ProcessMesh(mesh));
		}

		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			ProcessNode(aNode->mChildren[i], aScene);
		}
		return true;
	}

	Mesh Model::ProcessMesh(aiMesh* aMesh)
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indicies;
		std::vector<Bone> bones;

		for (uint32_t i = 0; i < aMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
			vertex.texCoords = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);

			//vertex.normal = glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < aMesh->mNumFaces; i++)
		{
			aiFace face = aMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indicies.push_back(face.mIndices[j]);
			}
		}

		if (aMesh->HasBones())
		{
			LoadBones(bones, vertices, aMesh);
		}

		return Mesh(vertices, indicies, bones);
	}

	void Model::LoadBones(std::vector<Bone>& aBones, std::vector<Vertex>& vertices, aiMesh* aMesh)
	{
		aBones.resize(aMesh->mNumBones);
		std::unordered_map<std::string, uint32_t> boneMapping;
		for (uint32_t boneId = 0; boneId < aMesh->mNumBones; boneId++)
		{
			auto name = std::string(aMesh->mBones[boneId]->mName.C_Str());
			boneMapping[name] = boneId;
		}
		
		for (uint32_t boneId = 0; boneId < aMesh->mNumBones; boneId++)
		{
			if (boneId != 0)
			{
				//std::string parentName = std::string(aMesh->mBones[boneId]->mNode->mParent->mName.C_Str());
				//aBones[0].parentIndex = boneMapping.find(parentName)->second;
			}
			auto test = aMesh->mBones[boneId];
			aBones[0].localMatrix = Math::ConvertAssimpMat4ToGlmMat4(aMesh->mBones[boneId]->mOffsetMatrix);
			for (uint32_t weightId = 0; weightId < aMesh->mBones[boneId]->mNumWeights; weightId++)
			{
				uint32_t vertexId = aMesh->mBones[boneId]->mWeights[weightId].mVertexId;
				float weight = aMesh->mBones[boneId]->mWeights[weightId].mWeight;
				for (uint32_t vertexBoneId = 0; vertexBoneId < ARRAYSIZE(vertices[vertexId].boneIDs); vertexBoneId++)
				{
					if (vertices[vertexId].weights[vertexBoneId] == 0.f)
					{
						vertices[vertexId].boneIDs[vertexBoneId] = boneId;
						vertices[vertexId].weights[vertexBoneId] = weight;
						break;
					}
				}
			}
		}
	}
}