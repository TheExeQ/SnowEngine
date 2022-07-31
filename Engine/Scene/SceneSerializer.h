#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include <yaml-cpp/yaml.h>

namespace Snow
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* aScene) { myScene = aScene; };
		~SceneSerializer() = default;

		void Serialize(const char* aFileName);
		bool Deserialize(const char* aFileName);

	private:
		void SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity);
			
		Scene* myScene;
	};
}