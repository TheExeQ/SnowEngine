#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Mesh.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <stdint.h>
#include <string>
#include <vector>

namespace Snow
{
	struct Frame
	{
		std::vector<glm::mat4> transforms;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const Animation& aAnim) = default;
		~Animation() = default;

		void PlayAnimation();
		void StopAnimation();

		void SetLoop(bool aActive);
		bool LoadAnimation(const char* aFilepath);

	private:
		void CalculateFramePose(const std::vector<Bone>& aBones,const Frame& inFrame, Frame& outFrame);

		std::string myName = "";
		bool myIsPlaying = false;
		bool myIsLooping = false;
		uint32_t myFPS = 0;
		std::vector<Frame> myFrames;

		inline static std::unordered_map<std::string, Ref<Animation>> myAnimations;
	};
}