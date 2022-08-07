#include "Animation.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Snow
{
	bool Animation::LoadAnimation(const char* aFilepath)
	{
		auto it = myAnimations.find(std::string(aFilepath));
		if (it != myAnimations.end())
		{
			*this = *it->second.get();
			CORE_LOG_INFO("Model reused.");
			return true;
		}
		
		myImporter = CreateRef<Assimp::Importer>();

		myScene = myImporter->ReadFile(aFilepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (!myScene || !myScene->HasAnimations())
		{
			CORE_LOG_WARN(std::string("Failed to load animation: ") + std::string(aFilepath));
			return false;
		}

		myAnimation = myScene->mAnimations[0];
		myDuration = myAnimation->mDuration;
		myTicksPerSecond = myAnimation->mTicksPerSecond;
		
		myFilePath = aFilepath;
		myAnimations[std::string(aFilepath)] = CreateRef<Animation>(*this);
		return true;
	}
}
