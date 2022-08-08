#include "SceneSerializer.h"
#include "Engine/Debug/Log.h"
#include "Engine/Math/Math.h"
#include "Engine/Editor/SceneHierarchyPanel.h"
#include "Components.h"

#include <glm/glm.hpp>
#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

namespace Snow
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity)
	{
		auto idComp = aEntity.GetComponent<IDComponent>();
		outEmitter << YAML::BeginMap;
		outEmitter << YAML::Key << "Entity" << YAML::Value << (uint64_t)idComp->uuid;
		
		if (aEntity.HasComponent<RelationshipComponent>())
		{
			auto comp = aEntity.GetComponent<RelationshipComponent>();
			outEmitter << YAML::Key << "Parent" << YAML::Value << (uint64_t)comp->Parent;
		}
		
		if (aEntity.HasComponent<TagComponent>())
		{
			outEmitter << YAML::Key << "TagComponent";
			outEmitter << YAML::BeginMap;
			auto comp = aEntity.GetComponent<TagComponent>();
			outEmitter << YAML::Key << "Tag" << YAML::Value << comp->name;
			outEmitter << YAML::EndMap;
		}

		if (aEntity.HasComponent<TransformComponent>())
		{
			auto comp = aEntity.GetComponent<TransformComponent>();
			glm::vec3 worldPosition;
			glm::vec3 worldRotation;
			glm::vec3 worldScale;
			
			Math::DecomposeTransform(myScene->GetWorldSpaceTransformMatrix(aEntity), worldPosition, worldRotation, worldScale);

			outEmitter << YAML::Key << "TransformComponent";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "Position" << YAML::Value << worldPosition;
			outEmitter << YAML::Key << "Rotation" << YAML::Value << worldRotation;
			outEmitter << YAML::Key << "Scale" << YAML::Value << worldScale;
			outEmitter << YAML::EndMap;
		}

		if (aEntity.HasComponent<StaticMeshComponent>())
		{
			outEmitter << YAML::Key << "StaticMeshComponent";
			outEmitter << YAML::BeginMap;
			auto comp = aEntity.GetComponent<StaticMeshComponent>();
			
			outEmitter << YAML::Key << "Model";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "MeshPath" << YAML::Value << comp->model.myFilePath;
			outEmitter << YAML::EndMap;
			
			outEmitter << YAML::Key << "Material";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "AlbedoPath" << YAML::Value << comp->material.myAlbedo.myFilePath;
			outEmitter << YAML::EndMap;
			
			outEmitter << YAML::EndMap;
		}

		if (aEntity.HasComponent<SkeletalMeshComponent>())
		{
			outEmitter << YAML::Key << "SkeletalMeshComponent";
			outEmitter << YAML::BeginMap;
			auto comp = aEntity.GetComponent<SkeletalMeshComponent>();

			outEmitter << YAML::Key << "Model";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "MeshPath" << YAML::Value << comp->animatedModel.myFilePath;
			outEmitter << YAML::EndMap;

			outEmitter << YAML::Key << "Material";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "AlbedoPath" << YAML::Value << comp->material.myAlbedo.myFilePath;
			outEmitter << YAML::EndMap;

			outEmitter << YAML::Key << "Animation";
			outEmitter << YAML::BeginMap;
			outEmitter << YAML::Key << "AnimationPath" << YAML::Value << comp->animation.myFilePath;
			outEmitter << YAML::EndMap;

			outEmitter << YAML::EndMap;
		}

		if (aEntity.HasComponent<CameraComponent>())
		{
			outEmitter << YAML::Key << "CameraComponent";
			outEmitter << YAML::BeginMap;
			auto comp = aEntity.GetComponent<CameraComponent>();

			outEmitter << YAML::Key << "Primary" << YAML::Value << comp->camera->myIsPrimary;
			outEmitter << YAML::Key << "FOV" << YAML::Value << comp->camera->myFov;
			outEmitter << YAML::Key << "Near" << YAML::Value << comp->camera->myNearPlane;
			outEmitter << YAML::Key << "Far" << YAML::Value << comp->camera->myFarPlane;

			outEmitter << YAML::EndMap;
		}

		if (aEntity.HasComponent<NativeScriptComponent>())
		{
			outEmitter << YAML::Key << "NativeScriptComponent";
			outEmitter << YAML::BeginMap;
			auto comp = aEntity.GetComponent<NativeScriptComponent>();

			outEmitter << YAML::Key << "ScriptID" << YAML::Value << comp->scriptID;

			outEmitter << YAML::EndMap;
		}

		outEmitter << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const char* aFileName)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << myScene->GetName();
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		myScene->myRegistry.each([&](entt::entity aEntity) 
			{
				Entity ent(aEntity, myScene.get());
				SerializeEntity(out, ent);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(aFileName);
		fout << out.c_str();
		
		CORE_LOG_INFO("Scene saved.");
	}

	bool SceneSerializer::Deserialize(const char* aFileName)
	{
		std::ifstream stream(aFileName);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		
		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"]) 
		{
			CORE_LOG_WARN("Could not load scene");
			return false; 
		}
		
		myScene->myRegistry.clear();

		auto entities = data["Entities"];
		if (entities)
		{
			std::vector<std::pair<Entity, uint64_t>> childEntities;
			
			for (auto ent : entities)
			{
				Entity DeserializedEntity(myScene.get());
				if (ent["TagComponent"])
				{
					auto tag = ent["TagComponent"]["Tag"];
					DeserializedEntity = myScene->CreateEntity(tag.as<std::string>().c_str());
					DeserializedEntity.GetComponent<IDComponent>()->uuid = ent["Entity"].as<uint64_t>();
					if (ent["Parent"].as<uint64_t>())
					{
						childEntities.push_back(std::pair<Entity, uint64_t>(DeserializedEntity, ent["Parent"].as<uint64_t>()));
					}
				}
				else { continue; }

				if (ent["TransformComponent"])
				{
					auto position = ent["TransformComponent"]["Position"];
					auto rotation = ent["TransformComponent"]["Rotation"];
					auto scale = ent["TransformComponent"]["Scale"];
					auto comp = DeserializedEntity.AddComponent<TransformComponent>();
					comp->position = position.as<glm::vec3>();
					comp->rotation = rotation.as<glm::vec3>(); 
					comp->scale = scale.as<glm::vec3>();
				}

				if (ent["StaticMeshComponent"])
				{
					auto model = ent["StaticMeshComponent"]["Model"];
					auto material = ent["StaticMeshComponent"]["Material"];
					auto comp = DeserializedEntity.AddComponent<StaticMeshComponent>();
					comp->model.LoadModel(model["MeshPath"].as<std::string>().c_str());
					comp->material.SetAlbedo(material["AlbedoPath"].as<std::string>().c_str());
				}

				if (ent["SkeletalMeshComponent"])
				{
					auto model = ent["SkeletalMeshComponent"]["Model"];
					auto material = ent["SkeletalMeshComponent"]["Material"];
					auto animation = ent["SkeletalMeshComponent"]["Animation"];
					auto comp = DeserializedEntity.AddComponent<SkeletalMeshComponent>();
					comp->animatedModel.LoadModel(model["MeshPath"].as<std::string>().c_str());
					comp->material.SetAlbedo(material["AlbedoPath"].as<std::string>().c_str());
					comp->animation.LoadAnimation(animation["AnimationPath"].as<std::string>().c_str());
				}

				if (ent["CameraComponent"])
				{
					auto primary = ent["CameraComponent"]["Primary"];
					auto fov = ent["CameraComponent"]["FOV"];
					auto nearPlane = ent["CameraComponent"]["Near"];
					auto farPlane = ent["CameraComponent"]["Far"];
					auto comp = DeserializedEntity.AddComponent<CameraComponent>();
					comp->camera->myIsPrimary = primary.as<bool>();
					comp->camera->myFov = fov.as<float>();
					comp->camera->myNearPlane = nearPlane.as<float>();
					comp->camera->myFarPlane = farPlane.as<float>();
				}

				if (ent["NativeScriptComponent"])
				{
					auto scriptID = ent["NativeScriptComponent"]["ScriptID"];
					auto comp = DeserializedEntity.AddComponent<NativeScriptComponent>();
					comp->scriptID = scriptID.as<int>();
					SceneHierarchyPanel::BindNativeScript(DeserializedEntity, scriptID.as<int>());
				}
			}
			
			for (auto entity : childEntities)
			{
				myScene->ParentEntity(entity.first, Entity(entity.second, myScene.get()));
			}
		}

		return true;
	}
}