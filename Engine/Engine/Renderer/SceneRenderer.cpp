#include "SceneRenderer.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Components.h"

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

	void SceneRenderer::UpdateEditorCamera(Entity aCamera)
	{
		myEditorCameraEntity = aCamera;
		
	}

	void SceneRenderer::UpdateRuntimeCamera(Entity aCamera)
	{
		myRuntimeCameraEntity = aCamera;
	}

	void SceneRenderer::RenderEditorScene()
	{
		auto sceneState = Engine::GetActiveScene()->GetSceneState();
		auto camera = myEditorCameraEntity.GetComponent<CameraComponent>()->camera;
		RenderStaticMeshes(camera);
		RenderSkeletalMeshes(camera, (sceneState == SceneState::Edit || sceneState == SceneState::Pause) ? false : true);
	}

	void SceneRenderer::RenderRuntimeScene()
	{
		auto camera = myRuntimeCameraEntity.GetComponent<CameraComponent>()->camera;
		RenderStaticMeshes(camera);
		RenderSkeletalMeshes(camera);
	}

	void SceneRenderer::RenderStaticMeshes(Ref<Camera> aCamera)
	{
		auto staticMeshEntities = Engine::GetActiveScene()->myRegistry.view<StaticMeshComponent>();

		for (auto& entity : staticMeshEntities)
		{
			Entity object(entity, Engine::GetActiveScene());
			const auto& meshComp = object.GetComponent<StaticMeshComponent>();

			auto objMatrix = object.GetWorldTransform();

			myFrameBuffer.myData.ViewMatrix = aCamera->GetViewMatrix();
			myFrameBuffer.myData.ProjectionMatrix = aCamera->GetProjectionMatrix();
			myFrameBuffer.ApplyChanges();
			
			myObjectBuffer.myData.WorldTransform = objMatrix;
			myObjectBuffer.myData.BoneTransforms.fill(glm::mat4(1.f));
			myObjectBuffer.ApplyChanges();

			DX11::Context->PSSetShaderResources(0, 1, meshComp->material.myAlbedo.myTextureView.GetAddressOf());
			for (auto mesh : meshComp->model.myMeshes)
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

	void SceneRenderer::RenderSkeletalMeshes(Ref<Camera> aCamera, bool aAnimateActivated)
	{
		auto skeletalMeshEntities = Engine::GetActiveScene()->myRegistry.view<SkeletalMeshComponent>();

		for (auto& entity : skeletalMeshEntities)
		{
			Entity object(entity, Engine::GetActiveScene());
			const auto& meshComp = object.GetComponent<SkeletalMeshComponent>();
			if (aAnimateActivated)
			{
				meshComp->animatedModel.SetAnimation(meshComp->animation);

				// Time only for testing until animation works
				static float time = 0.f;
				time += Time::GetDeltaTime() * 3.f;
				if (time > meshComp->animation.myDuration) { time = 0.f; }

				meshComp->animatedModel.UpdateBoneTransform(time);
			}
			auto objMatrix = object.GetWorldTransform();

			myFrameBuffer.myData.ViewMatrix = aCamera->GetViewMatrix();
			myFrameBuffer.myData.ProjectionMatrix = aCamera->GetProjectionMatrix();
			myFrameBuffer.ApplyChanges();

			myObjectBuffer.myData.WorldTransform = objMatrix;
			myObjectBuffer.myData.BoneTransforms.fill(glm::mat4(1.f));
			if (aAnimateActivated)
			{
				for (uint32_t i = 0; i < meshComp->animatedModel.myBoneTransforms.size(); i++)
				{
					myObjectBuffer.myData.BoneTransforms[i] = meshComp->animatedModel.myBoneTransforms[i];
				}
			}
			myObjectBuffer.ApplyChanges();

			DX11::Context->PSSetShaderResources(0, 1, meshComp->material.myAlbedo.myTextureView.GetAddressOf());
			for (auto mesh : meshComp->animatedModel.myMeshes)
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
}