#include "Animation.h"
#include "Engine/Debug/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Snow
{
	void Animation::PlayAnimation()
	{
		myIsPlaying = true;
	}

	void Animation::StopAnimation()
	{
		myIsPlaying = false;
	}

	void Animation::SetLoop(bool aActive)
	{
		myIsLooping = aActive;
	}

	bool Animation::LoadAnimation(const char* aFilepath)
	{
		auto it = myAnimations.find(std::string(aFilepath));
		if (it != myAnimations.end())
		{
			myName = it->second->myName;
			myIsPlaying = it->second->myIsPlaying;
			myIsLooping = it->second->myIsLooping;
			myFPS = it->second->myFPS;
			myFrames = it->second->myFrames;
			CORE_LOG_INFO("Animation reused!");
			return true;
		}
		
		// Do assimp loading code

		myAnimations[std::string(aFilepath)] = CreateRef<Animation>(*this);
	}

	void Animation::CalculateFramePose(const std::vector<Bone>& aBones, const Frame& inFrame, Frame& outFrame)
	{
		std::vector<glm::mat4> localTransform(aBones.size());
		std::vector<glm::mat4> modelTransform(aBones.size());
		
		for (uint32_t i = 0; i < aBones.size(); i++)
		{
			localTransform[i] = inFrame.transforms[i] * aBones[i].localMatrix;
		}

		modelTransform[0] = localTransform[0];

		for (uint32_t i = 1; i < aBones.size(); i++)
		{
			uint32_t parent = aBones[i].parentIndex;
			modelTransform[i] = localTransform[i] * modelTransform[parent];
		}

		for (uint32_t i = 0; i < aBones.size(); i++)
		{
			outFrame.transforms[i] = aBones[i].inverseModelMatrix * modelTransform[i];
		}
	}
}