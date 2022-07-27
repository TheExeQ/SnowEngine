#pragma once
#include "DX11.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"

#include "Mesh.h"

class Renderer
{
private:
	friend class Engine;

	bool Initialize(HWND hwnd, const int& aWidth, const int& aHeight);
	void BeginFrame();
	void EndFrame();

	bool CreateShaders();
	bool CreateConstantBuffers();
	bool InitScene();
	
	VertexShader myVertexShader;
	PixelShader myPixelShader;
	
	ConstantBuffer<FrameBuffer> myFrameBuffer;

	Camera myMainCamera;

	DX11 myGraphicsAPI;
	bool myVSyncEnabled = true;
};

