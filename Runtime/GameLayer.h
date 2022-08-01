#pragma once
#include <Engine/Core/Layer.h>
#include <Engine/Scene/Entity.h>

namespace Snow
{
	class GameLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUpdate() override;
	
	private:
		Entity myEntity;
	};
}
