#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Debug/Log.h"

#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/base/containers/vector.h"
#include "ozz/options/options.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace Snow
{
	class Skeleton // #TODO: Add to asset class that handles loading and caching
	{
	public:
		bool LoadSkeleton(const char* aFilepath);
		bool LoadAnimation(const char* aFilepath);
		bool OnUpdate();

	private:
		friend class SceneRenderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		
		float myTimeRatio = 0.0f;
		float myPlaybackSpeed = 1.f;
		Ref<ozz::animation::Animation> myAnimation = CreateRef<ozz::animation::Animation>();
		Ref<ozz::animation::Skeleton> mySkeleton = CreateRef<ozz::animation::Skeleton>();

		Ref<ozz::animation::SamplingJob::Context> myContext = CreateRef<ozz::animation::SamplingJob::Context>();
		ozz::vector<ozz::math::SoaTransform> myLocalTransforms;
		ozz::vector<ozz::math::Float4x4> myModelTransforms;
		std::string mySkeletonFilePath = "";
		std::string myAnimationFilePath = "";
	};
}