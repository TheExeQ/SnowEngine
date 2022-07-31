#pragma once
#include <Core/Layer.h>
#include <Scene/Entity.h>

namespace Snow
{
	class GameLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUpdate() override;

	private:
		Entity myCube;
		Entity myChest;
		Entity myPyramid;
	};
}
