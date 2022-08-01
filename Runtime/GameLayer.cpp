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

		Engine::GetActiveScene()->LoadScene("../Assets/Scenes/test.scene");
		myEntity = Entity(entt::entity(1), Engine::GetActiveScene());
	}

	void GameLayer::OnUpdate()
	{
		if (InputManager::Get()->IsKeyDown(Key::W))
		{
			myEntity.GetComponent<TransformComponent>()->position.z -= 1.f * Time::GetDeltaTime();
		}
		if (InputManager::Get()->IsKeyDown(Key::A))
		{
			myEntity.GetComponent<TransformComponent>()->position.x += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::Get()->IsKeyDown(Key::S))
		{
			myEntity.GetComponent<TransformComponent>()->position.z += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::Get()->IsKeyDown(Key::D))
		{
			myEntity.GetComponent<TransformComponent>()->position.x -= 1.f * Time::GetDeltaTime();
		}
		if (InputManager::Get()->IsKeyDown(Key::SPACE))
		{
			myEntity.GetComponent<TransformComponent>()->position.y += 1.f * Time::GetDeltaTime();
		}
		if (InputManager::Get()->IsKeyDown(Key::CONTROL))
		{
			myEntity.GetComponent<TransformComponent>()->position.y -= 1.f * Time::GetDeltaTime();
		}
	}
}