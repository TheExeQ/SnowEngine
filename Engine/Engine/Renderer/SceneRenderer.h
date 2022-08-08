#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Scene/Entity.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

namespace Snow
{
	class Camera;
	
	class SceneRenderer
	{
	public:
		bool Initialize();
		void UpdateEditorCamera(Entity aCamera);
		void UpdateRuntimeCamera(Entity aCamera);

		void RenderEditorScene();
		void RenderRuntimeScene();

	private:
		friend class SceneViewportPanel;
		void RenderStaticMeshes(Entity& aCamera);
		void RenderSkeletalMeshes(Entity& aCamera, bool aAnimateActivated = true);

		bool CreateShaders();
		bool CreateConstantBuffers();

		VertexShader myVertexShader;
		PixelShader myPixelShader;

		ConstantBuffer<FrameBuffer> myFrameBuffer;
		ConstantBuffer<ObjectBuffer> myObjectBuffer;

		Entity myEditorCameraEntity;
		Entity myRuntimeCameraEntity;
	};
}