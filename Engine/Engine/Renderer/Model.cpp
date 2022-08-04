#include "Model.h"
#include "Engine/Core/Random.h"
#include "Engine/Debug/Log.h"

namespace Snow
{
	bool Model::Initialize(const char* filepath)
	{
		if (!LoadModel(filepath))
		{
			return false;
		}
		return true;
	}

	void Model::Initialize(Mesh aMesh)
	{
		myMeshes.clear();
		myMeshes.push_back(aMesh);
	}

	bool Model::LoadModel(const char* aFilepath)
	{
		auto it = myModels.find(std::string(aFilepath));
		if (it != myModels.end())
		{
			myFilePath = it->second->myFilePath;
			myMeshes = it->second->myMeshes;
			CORE_LOG_INFO("Model reused!");
			return true;
		}

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(aFilepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

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
		for (UINT i = 0; i < aNode->mNumMeshes; i++)
		{
			aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
			myMeshes.push_back(ProcessMesh(mesh));
		}

		for (UINT i = 0; i < aNode->mNumChildren; i++)
		{
			ProcessNode(aNode->mChildren[i], aScene);
		}
		return true;
	}

	Mesh Model::ProcessMesh(aiMesh* aMesh)
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indicies;

		for (UINT i = 0; i < aMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
			vertex.texCoords = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);

			//vertex.normal = XMFLOAT3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
			vertices.push_back(vertex);
		}

		for (UINT i = 0; i < aMesh->mNumFaces; i++)
		{
			aiFace face = aMesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indicies.push_back(face.mIndices[j]);
			}
		}
		return Mesh(vertices, indicies);
	}
}