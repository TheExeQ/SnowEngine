#include "SceneRenderer.h"
#include "Engine/Engine.h"
#include "Engine/Core/Time.h"
#include "Engine/Math/Math.h"
#include "Engine/Scene/Components.h"

#include "Engine/Debug/Profiler.h"

#include <iostream>

namespace Snow
{
	bool SceneRenderer::Initialize()
	{
		if (!CreateShaders()) { return false; }
		if (!CreateConstantBuffers()) { return false; }
		return true;
	}

	bool SceneRenderer::CreateShaders()
	{
		//Create the input assembler
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (!myVertexShader.Init(L"../bin/Shaders/Default-vs.cso", inputLayoutDesc, ARRAYSIZE(inputLayoutDesc)))
		{
			return false;
		};

		if (!myPixelShader.Init(L"../bin/Shaders/Default-ps.cso"))
		{
			return false;
		};
		return true;
	}

	bool SceneRenderer::CreateConstantBuffers()
	{
		HRESULT hr;
		hr = myFrameBuffer.Init(DX11::Device.Get(), DX11::Context.Get());
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create constant buffer");
			return false;
		}

		hr = myFrameBuffer.ApplyChanges();
		if (FAILED(hr))
		{
			CORE_LOG_WARN("Failed to update constant buffer");
			return false;
		}

		hr = myObjectBuffer.Init(DX11::Device.Get(), DX11::Context.Get());
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("Failed to create constant buffer");
			return false;
		}

		hr = myObjectBuffer.ApplyChanges();
		if (FAILED(hr))
		{
			CORE_LOG_WARN("Failed to update constant buffer");
			return false;
		}
		return true;
	}

	void SceneRenderer::RenderEditorScene()
	{
		auto sceneState = Engine::GetActiveScene()->GetSceneState();
		if (sceneState == SceneState::Play)
		{
			auto camEntity = Engine::GetActiveScene()->GetPrimaryCameraEntity();
			if (camEntity.IsValid())
			{
				RenderStaticMeshes(camEntity);
				RenderSkeletalMeshes(camEntity, (sceneState == SceneState::Edit || sceneState == SceneState::Pause) ? false : true);
				//RenderBones(camEntity, (sceneState == SceneState::Edit || sceneState == SceneState::Pause) ? false : true);
			}
		}
		else
		{
			auto camEntity = Engine::GetEditorScene()->GetPrimaryCameraEntity();
			if (camEntity.IsValid())
			{
				RenderStaticMeshes(camEntity);
				RenderSkeletalMeshes(camEntity, (sceneState == SceneState::Edit || sceneState == SceneState::Pause) ? false : true);
				//RenderBones(camEntity, (sceneState == SceneState::Edit || sceneState == SceneState::Pause) ? false : true);
			}
		}
	}

	void SceneRenderer::RenderRuntimeScene()
	{
		auto camEntity = Engine::GetActiveScene()->GetPrimaryCameraEntity();

		if (camEntity.IsValid())
		{
			RenderStaticMeshes(camEntity);
			RenderSkeletalMeshes(camEntity);
		}
	}

	void SceneRenderer::RenderStaticMeshes(Entity& aCamera)
	{
		auto staticMeshEntities = Engine::GetActiveScene()->myRegistry.view<StaticMeshComponent>();

		for (auto& entity : staticMeshEntities)
		{
			Entity object(entity, Engine::GetActiveScene().get());
			
			const auto& meshComp = object.GetComponent<StaticMeshComponent>();
			auto cameraTransform = aCamera.GetWorldTransform();
			glm::vec3 camPos;
			glm::vec3 camRot;
			glm::vec3 camScale;
			Math::DecomposeTransform(cameraTransform, camPos, camRot, camScale);
			
			auto& camera = aCamera.GetComponent<CameraComponent>().camera;

			auto objMatrix = object.GetWorldTransform();

			myFrameBuffer.myData.ViewMatrix = camera.GetViewMatrix(camPos, camRot);
			myFrameBuffer.myData.ProjectionMatrix = camera.GetProjectionMatrix();
			myFrameBuffer.ApplyChanges();

			myObjectBuffer.myData.WorldTransform = objMatrix;
			myObjectBuffer.myData.BoneTransforms.fill(glm::mat4(1.f));
			myObjectBuffer.ApplyChanges();

			DX11::Context->PSSetShaderResources(0, 1, meshComp.material.myAlbedo.myTextureView.GetAddressOf());
			for (auto mesh : meshComp.model.myMeshes)
			{
				DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

				DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
				DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

				DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

				UINT offset = 0;

				DX11::Context->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), mesh.myVertexBuffer.StridePtr(), &offset);
				DX11::Context->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->DrawIndexed(mesh.myIndexBuffer.BufferSize(), 0, 0);
			}
		}
	}

	void SceneRenderer::RenderSkeletalMeshes(Entity& aCamera, bool aAnimateActivated)
	{
		auto skeletalMeshEntities = Engine::GetActiveScene()->myRegistry.view<SkeletalMeshComponent>();

		for (auto& entity : skeletalMeshEntities)
		{
			Entity object(entity, Engine::GetActiveScene().get());
			auto& meshComp = object.GetComponent<SkeletalMeshComponent>();
			
			auto cameraTransform = aCamera.GetWorldTransform();
			glm::vec3 camPos;
			glm::vec3 camRot;
			glm::vec3 camScale;
			Math::DecomposeTransform(cameraTransform, camPos, camRot, camScale);
			
			auto& camera = aCamera.GetComponent<CameraComponent>().camera;
			auto objMatrix = object.GetWorldTransform();

			myFrameBuffer.myData.ViewMatrix = camera.GetViewMatrix(camPos, camRot);
			myFrameBuffer.myData.ProjectionMatrix = camera.GetProjectionMatrix();
			myFrameBuffer.ApplyChanges();

			myObjectBuffer.myData.WorldTransform = objMatrix;
			myObjectBuffer.myData.BoneTransforms.fill(glm::mat4(1.f));
			if (aAnimateActivated)
			{
				meshComp.skeleton.OnUpdate();
				for (uint32_t i = 0; i < meshComp.skeleton.myModelTransforms.size(); i++)
				{
					auto mat = Math::ConvertOzzMat4ToGlmMat4(meshComp.skeleton.myModelTransforms[i]) * meshComp.animatedModel.myBindPoseInverses[i];
					myObjectBuffer.myData.BoneTransforms[i] = glm::transpose(mat);
				}
			}
			myObjectBuffer.ApplyChanges();

			DX11::Context->PSSetShaderResources(0, 1, meshComp.material.myAlbedo.myTextureView.GetAddressOf());
			for (auto mesh : meshComp.animatedModel.myMeshes)
			{
				DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

				DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
				DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

				DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

				UINT offset = 0;

				DX11::Context->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), mesh.myVertexBuffer.StridePtr(), &offset);
				DX11::Context->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->DrawIndexed(mesh.myIndexBuffer.BufferSize(), 0, 0);
			}
		}
	}

	void SceneRenderer::RenderBones(Entity& aCamera, bool aAnimateActivated /*= true*/)
	{
		auto skeletalMeshEntities = Engine::GetActiveScene()->myRegistry.view<SkeletalMeshComponent>();

		std::vector<Vertex> vertices =
		{
			// Front
			Vertex(-0.5f, -0.5f, -0.5f, 0.f, 0.f), //Left Bottom
			Vertex(-0.5f, 0.5f, -0.5f, 0.f, 0.f), //Left Top
			Vertex(0.5f, 0.5f, -0.5f, 0.f, 0.f), //Right Top
			Vertex(0.5f, -0.5f, -0.5f, 0.f, 0.f), //Right Bottom

			// Back
			Vertex(-0.5f, -0.5f, 0.5f, 0.f, 0.f), //Left Bottom
			Vertex(-0.5f, 0.5f, 0.5f, 0.f, 0.f), //Left Top
			Vertex(0.5f, 0.5f, 0.5f, 0.f, 0.f), //Right Top
			Vertex(0.5f, -0.5f, 0.5f, 0.f, 0.f), //Right Bottom
		};

		std::vector<DWORD> indicies =
		{
			//Front
			0, 1, 2,
			0, 2, 3,

			// Back
			6, 5, 4,
			7, 6, 4,

			// Left
			0, 4, 5,
			0, 5, 1,

			// Right
			2, 6, 7,
			2, 7, 3,

			// Top
			1, 5, 6,
			1, 6, 2,

			// Bottom
			4, 0, 3,
			4, 3, 7,
		};

		Mesh cube(vertices, indicies);

		for (auto& entity : skeletalMeshEntities)
		{
			Entity object(entity, Engine::GetActiveScene().get());
			auto& meshComp = object.GetComponent<SkeletalMeshComponent>();

			auto cameraTransform = aCamera.GetWorldTransform();
			glm::vec3 camPos;
			glm::vec3 camRot;
			glm::vec3 camScale;
			Math::DecomposeTransform(cameraTransform, camPos, camRot, camScale);

			auto& camera = aCamera.GetComponent<CameraComponent>().camera;
			auto objTransform = glm::scale(object.GetWorldTransform(), glm::vec3(100.f));

			myFrameBuffer.myData.ViewMatrix = camera.GetViewMatrix(camPos, camRot);
			myFrameBuffer.myData.ProjectionMatrix = camera.GetProjectionMatrix();
			myFrameBuffer.ApplyChanges();

			myObjectBuffer.myData.BoneTransforms.fill(glm::mat4(1.f));
			DX11::Context->PSSetShaderResources(0, 1, meshComp.material.myAlbedo.myTextureView.GetAddressOf());
			meshComp.skeleton.OnUpdate();
			for (uint32_t i = 0; i < meshComp.skeleton.myModelTransforms.size(); i++)
			{
				auto transform = Math::ConvertOzzMat4ToGlmMat4(meshComp.skeleton.myModelTransforms[i]);
				transform = glm::scale(transform, glm::vec3(0.015f, 0.015f, 0.015f));
				transform = objTransform * transform;

				myObjectBuffer.myData.WorldTransform = transform;
				myObjectBuffer.ApplyChanges();

				DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX11::Context->IASetInputLayout(myVertexShader.GetInputLayout());

				DX11::Context->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
				DX11::Context->PSSetShader(myPixelShader.GetShader(), nullptr, 0);

				DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
				DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

				UINT offset = 0;

				DX11::Context->IASetVertexBuffers(0, 1, cube.myVertexBuffer.GetAddressOf(), cube.myVertexBuffer.StridePtr(), &offset);
				DX11::Context->IASetIndexBuffer(cube.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				DX11::Context->DrawIndexed(cube.myIndexBuffer.BufferSize(), 0, 0);
			}
		}
	}
}