#include "GameLayer.h"
#include <Engine.h>
#include <Renderer/MeshFactory.h>
#include <iostream>

namespace Snow
{
	void GameLayer::OnAttach()
	{
		std::cout << "Game Layer Attached" << std::endl;

		myCube = Engine::GetActiveScene()->CreateEntity("Cube");
		myCube.GetComponent<TransformComponent>()->position = { 2.0f, 0.0f, 0.0f };
		myCube.GetComponent<TransformComponent>()->rotation = { 0.f, 0.0f, 0.0f };
		myCube.AddComponent<StaticMeshComponent>()->model.Initialize(MeshFactory::CreateCube());

		myChest = Engine::GetActiveScene()->CreateEntity("Chest");
		myChest.GetComponent<TransformComponent>()->position = { -2.0f, -0.5f, 0.0f };
		myChest.GetComponent<TransformComponent>()->rotation = { 0.f, glm::radians(-45.0f), 0.0f };
		myChest.GetComponent<TransformComponent>()->scale = { 0.01f, 0.01f, 0.01f };
		myChest.AddComponent<StaticMeshComponent>()->model.Initialize("../Assets/Models/SM/Particle_Chest.fbx");
		myChest.GetComponent<StaticMeshComponent>()->material.SetAlbedo("../Assets/Textures/T_Particle_Chest_C.png");

		myPyramid = Engine::GetActiveScene()->CreateEntity("Pyramid");
		myPyramid.GetComponent<TransformComponent>()->position = { 0.0f, 0.0f, 0.0f };
		myPyramid.GetComponent<TransformComponent>()->rotation = { 0.0f, 0.0f, 0.0f };
		myPyramid.AddComponent<StaticMeshComponent>()->model.Initialize(MeshFactory::CreatePyramid());
	}

	void GameLayer::OnUpdate()
	{
		if (myCube.HasComponent<TransformComponent>())
		{
			myCube.GetComponent<TransformComponent>()->rotation.z += glm::radians(0.5f);
		}

		if (myChest.HasComponent<TransformComponent>())
		{
			myChest.GetComponent<TransformComponent>()->rotation.y += glm::radians(0.5f);
		}

		if (myPyramid.HasComponent<TransformComponent>())
		{
			myPyramid.GetComponent<TransformComponent>()->rotation.y += glm::radians(0.5f);
		}
	}
}