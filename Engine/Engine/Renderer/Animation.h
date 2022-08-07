#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Debug/Log.h"

struct aiScene;
struct aiAnimation;

namespace Assimp
{
	class Importer;
}

namespace Snow
{
	class Animation
	{
	public:
		bool LoadAnimation(const char* aFilepath);
		const aiAnimation* GetAnimation() const { return myAnimation; };

	private:
		Ref<Assimp::Importer> myImporter;
		const aiScene* myScene;
		const aiAnimation* myAnimation;

		float myDuration = 0.f;
		int myTicksPerSecond = 0;
		std::string myFilePath = "";
		
		inline static std::unordered_map<std::string, Ref<Animation>> myAnimations;
	};
}