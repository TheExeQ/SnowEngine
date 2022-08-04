#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/DX11.h"
#include "Engine/Scene/Entity.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"

namespace Snow
{
	class Renderer
	{
	private:
		friend class Engine;
		friend class SceneViewportPanel;

		bool Initialize(HWND hwnd);
		void BeginFrame();
		void EndFrame();

		bool CreateShaders();
		bool CreateConstantBuffers();

		VertexShader myVertexShader;
		PixelShader myPixelShader;

		ConstantBuffer<FrameBuffer> myFrameBuffer;
		ConstantBuffer<ObjectBuffer> myObjectBuffer;

		Ref<Camera> myMainCamera = nullptr;

		DX11 myGraphicsAPI;
		bool myVSyncEnabled = true;
	};
}