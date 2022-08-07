#include "Model.h"
#include "Engine/Math/Math.h"
#include "Engine/Renderer/Animation.h"

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
			*this = *it->second.get();
			CORE_LOG_INFO("Model reused.");
			return true;
		}

		myImporter = CreateRef<Assimp::Importer>();

		myScene = myImporter->ReadFile(aFilepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (!myScene)
		{
			CORE_LOG_WARN(std::string("Failed to load model: ") + std::string(aFilepath));
			return false;
		}

		myMeshes.clear();

		myInverseTransform = glm::inverse(Math::ConvertAssimpMat4ToGlmMat4(myScene->mRootNode->mTransformation));
		ProcessNode(myScene->mRootNode, myScene);

		myFilePath = aFilepath;
		myModels[std::string(aFilepath)] = CreateRef<Model>(*this);
		return true;
	}

	void Model::SetAnimation(const Animation& aAnimation)
	{
		myCurrentAnimation = &aAnimation;
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
			LoadBones(vertices, aMesh);
		}

		return Mesh(vertices, indicies);
	}

	void Model::LoadBones(std::vector<Vertex>& vertices, aiMesh* aMesh)
	{
		std::vector<Bone> bones(aMesh->mNumBones);

		for (uint32_t boneId = 0; boneId < aMesh->mNumBones; boneId++)
		{
			auto name = std::string(aMesh->mBones[boneId]->mName.C_Str());
			myBoneMapping[name] = boneId;
		}
		
		for (uint32_t boneId = 0; boneId < aMesh->mNumBones; boneId++)
		{
			bones[boneId].offsetTransform = Math::ConvertAssimpMat4ToGlmMat4(aMesh->mBones[boneId]->mOffsetMatrix);
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

		myBones.insert(myBones.end(), bones.begin(), bones.end());
	}

	void Model::UpdateBoneTransform(float aTime)
	{
		if (myCurrentAnimation->GetAnimation())
		{
			ReadNodeHierarchy(aTime, myScene->mRootNode, glm::mat4(1.0f));
			myBoneTransforms.resize(myBones.size());
			for (size_t i = 0; i < myBones.size(); i++)
			{
				myBoneTransforms[i] = myBones[i].finalTransform;
			}
		}
	}

	const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName)
			{
				return nodeAnim;
			}
		}
		return nullptr;
	}

	uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}

	uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}

	uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}

	glm::vec3 InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumPositionKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mPositionKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
		uint32_t NextPositionIndex = (PositionIndex + 1);
		float DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		Factor = glm::clamp(Factor, 0.0f, 1.0f);
		const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		auto aiVec = Start + Factor * Delta;
		return { aiVec.x, aiVec.y, aiVec.z };
	}

	glm::quat InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumRotationKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mRotationKeys[0].mValue;
			return glm::quat(v.w, v.x, v.y, v.z);
		}

		uint32_t RotationIndex = FindRotation(animationTime, nodeAnim);
		uint32_t NextRotationIndex = (RotationIndex + 1);
		float DeltaTime = (float)(nodeAnim->mRotationKeys[NextRotationIndex].mTime - nodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		Factor = glm::clamp(Factor, 0.0f, 1.0f);
		const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
		auto q = aiQuaternion();
		aiQuaternion::Interpolate(q, StartRotationQ, EndRotationQ, Factor);
		q = q.Normalize();
		return glm::quat(q.w, q.x, q.y, q.z);
	}

	glm::vec3 InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumScalingKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mScalingKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32_t index = FindScaling(animationTime, nodeAnim);
		uint32_t nextIndex = (index + 1);
		float deltaTime = (float)(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
		float factor = (animationTime - (float)nodeAnim->mScalingKeys[index].mTime) / deltaTime;
		factor = glm::clamp(factor, 0.0f, 1.0f);
		const auto& start = nodeAnim->mScalingKeys[index].mValue;
		const auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
		auto delta = end - start;
		auto aiVec = start + factor * delta;
		return { aiVec.x, aiVec.y, aiVec.z };
	}

	void Model::ReadNodeHierarchy(float aAnimationTime, const aiNode* aNode, const glm::mat4& aParentTransform)
	{
		std::string name(aNode->mName.data);
		glm::mat4 nodeTransform(Math::ConvertAssimpMat4ToGlmMat4(aNode->mTransformation));
		const aiNodeAnim* nodeAnim = FindNodeAnim(myCurrentAnimation->GetAnimation(), name);

		if (nodeAnim)
		{
			glm::vec3 translation = InterpolateTranslation(aAnimationTime, nodeAnim);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

			glm::quat rotation = InterpolateRotation(aAnimationTime, nodeAnim);
			glm::mat4 rotationMatrix = glm::toMat4(rotation);

			glm::vec3 scale = InterpolateScale(aAnimationTime, nodeAnim);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));

			nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
		}

		glm::mat4 transform = aParentTransform * nodeTransform;

		if (myBoneMapping.find(name) != myBoneMapping.end())
		{
			uint32_t BoneIndex = myBoneMapping[name];
			myBones[BoneIndex].finalTransform = myInverseTransform * transform * myBones[BoneIndex].offsetTransform;
		}

		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			ReadNodeHierarchy(aAnimationTime, aNode->mChildren[i], transform);
		}
	}
}