#include "SceneViewportPanel.h"
#include "Engine/Core/DX11.h"
#include <imgui/imgui.h>

namespace Snow
{
	void SceneViewportPanel::Init()
	{
		if (!myInstace)
		{
			myInstace = this;
		}
		else
		{
			return;
		}

	}

	void SceneViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Viewport", &myOpened , ImGuiWindowFlags_NoCollapse);
		auto viewportPanelSize = ImGui::GetContentRegionAvail();
		if (myViewportSize != *(glm::vec2*)&viewportPanelSize)
		{
			myViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
			OnResize();
		}
		ImGui::Image(DX11::Get().myEditorShaderResourceView.Get(), ImVec2(myViewportSize.x, myViewportSize.y));
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::OnResize()
	{
		DX11::Get().myEditorTexture->Release();
		DX11::Get().myEditorRenderTargetView->Release();
		DX11::Get().myEditorShaderResourceView->Release();
		
		DX11::Get().myEditorDepthStencilTexture->Release();
		DX11::Get().myEditorDepthStencilState->Release();
		DX11::Get().myEditorDepthStencilView->Release();

		DX11::Get().CreateEditorRenderTargetView(myViewportSize.x, myViewportSize.y);
		DX11::Get().CreateEditorDepthStencil(myViewportSize.x, myViewportSize.y);
	}
}