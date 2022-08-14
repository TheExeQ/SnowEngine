#include "skeleton.h"
#include "Engine/Core/Time.h"

#include <ozz/base/log.h>
#include <ozz/base/io/stream.h>
#include <ozz/base/io/archive.h>
#include <ozz/framework/mesh.h>
#include <ozz/framework/utils.h>

namespace Snow
{
	bool Skeleton::LoadSkeleton(const char* aFilepath)
	{
		ozz::io::File file(aFilepath, "rb");

		if (!file.opened())
		{
			CORE_LOG_WARN(std::string("Could not load skeleton file: ") + aFilepath);
			return false;
		}

		ozz::io::IArchive archive(&file);

		if (!archive.TestTag<ozz::animation::Skeleton>())
		{
			CORE_LOG_WARN(std::string("Could not find skeleton tag: ") + aFilepath);
			return false;
		}

		archive >> *mySkeleton;
		
		const int num_joints = mySkeleton->num_joints();
		myLocalTransforms.resize(mySkeleton->num_soa_joints());
		myModelTransforms.resize(num_joints);

		myContext->Resize(num_joints);

		mySkeletonFilePath = aFilepath;
		return true;
	}

	bool Skeleton::LoadAnimation(const char* aFilepath)
	{
		ozz::io::File file(aFilepath, "rb");

		if (!file.opened())
		{
			CORE_LOG_WARN(std::string("Could not load animation file: ") + aFilepath);
			return false;
		}

		ozz::io::IArchive archive(&file);

		if (!archive.TestTag<ozz::animation::Animation>())
		{
			CORE_LOG_WARN(std::string("Could not find animation tag: ") + aFilepath);
			return false;
		}

		archive >> *myAnimation;
		myAnimationFilePath = aFilepath;
		return true;
	}

	bool Skeleton::OnUpdate()
	{
		myTimeRatio += Time::GetDeltaTime();
		if (myTimeRatio > 1.f) { myTimeRatio = 0.f; };

		// Samples optimized animation
		ozz::animation::SamplingJob sampling_job;
		sampling_job.animation = myAnimation.get();
		sampling_job.context = myContext.get();
		sampling_job.ratio = myTimeRatio;
		sampling_job.output = make_span(myLocalTransforms);
		if (!sampling_job.Run())
		{
			return false;
		}

		// Converts from local space to model space matrices.
		ozz::animation::LocalToModelJob ltm_job;
		ltm_job.skeleton = mySkeleton.get();
		ltm_job.input = make_span(myLocalTransforms);
		ltm_job.output = make_span(myModelTransforms);

		if (!ltm_job.Run())
		{
			return false;
		}

		return true;
	}
}