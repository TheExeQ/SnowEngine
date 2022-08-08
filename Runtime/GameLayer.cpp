#include "GameLayer.h"
#include <Engine/Engine.h>
#include <Engine/Debug/Log.h>

namespace Snow
{
	void GameLayer::OnAttach()
	{
		LOG_INFO("Game Layer Attached");

		Engine::GetActiveScene()->LoadScene("../Assets/Scenes/gametest.scene");
		Engine::GetActiveScene()->OnRuntimeStart();
	}
}