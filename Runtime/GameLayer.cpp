#include "GameLayer.h"
#include <Engine/Engine.h>
#include <Engine/Renderer/MeshFactory.h>
#include <Engine/Scene/Scene.h>

#include <iostream>

namespace Snow
{
	void GameLayer::OnAttach()
	{
		std::cout << "Game Layer Attached" << std::endl;

		Engine::GetActiveScene()->LoadScene("../Assets/Scenes/gametest.scene");
		myEntity = Entity(UUID(1748500495750464391), Engine::GetActiveScene());
	}

	void GameLayer::OnUpdate()
	{
		if (InputManager::IsKeyDown(Key::W))
		{
			myEntity.GetComponent<TransformComponent>()->position.z += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::IsKeyDown(Key::A))
		{
			myEntity.GetComponent<TransformComponent>()->position.x -= 1.f * Time::GetDeltaTime();
		}
		if (InputManager::IsKeyDown(Key::S))
		{
			myEntity.GetComponent<TransformComponent>()->position.z -= 1.f * Time::GetDeltaTime();
		}
		if (InputManager::IsKeyDown(Key::D))
		{
			myEntity.GetComponent<TransformComponent>()->position.x += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::IsKeyDown(Key::SPACE))
		{
			myEntity.GetComponent<TransformComponent>()->position.y += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::IsKeyDown(Key::CONTROL))
		{
			myEntity.GetComponent<TransformComponent>()->position.y -= 1.f * Time::GetDeltaTime();
		}
	}
}