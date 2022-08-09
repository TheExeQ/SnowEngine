#pragma once
#include "Engine/Core/Fundamentals.h"
#include "Engine/Scene/ScriptableEntity.h"

using namespace Snow;

namespace Game
{
	class CharacterScript : public ScriptableEntity
	{
	public:
		void OnUpdate() 
		{  
			auto& transform = GetComponent<TransformComponent>();
			if (InputManager::IsKeyDown(Key::W))
			{
				transform.position += transform.Forward() * Time::GetDeltaTime() * mySpeed;
			}
			if (InputManager::IsKeyDown(Key::S))
			{
				transform.position -= transform.Forward() * Time::GetDeltaTime() * mySpeed;
			}
			if (InputManager::IsKeyDown(Key::D))
			{
				transform.position += transform.Right() * Time::GetDeltaTime() * mySpeed;
			}
			if (InputManager::IsKeyDown(Key::A))
			{
				transform.position -= transform.Right() * Time::GetDeltaTime() * mySpeed;
			}
			if (InputManager::IsKeyDown(Key::E))
			{
				transform.rotation.y -= glm::radians(20.f) * Time::GetDeltaTime() * mySpeed;
			}
			if (InputManager::IsKeyDown(Key::Q))
			{
				transform.rotation.y += glm::radians(20.f) * Time::GetDeltaTime() * mySpeed;
			}
		};

	private:
		float mySpeed = 5.f;
	};
}
