#include "SceneViewportPanel.h"
#include "Engine/Core/DX11.h"
#include "Engine/Engine.h"
#include "Engine/Math/Math.h"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>

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

	void SceneViewportPanel::OnImGuiRender(Entity aSelectedEntity)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Viewport", &myOpened , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
		auto viewportPanelSize = ImGui::GetContentRegionAvail();
		if (myViewportSize != *(glm::vec2*)&viewportPanelSize)
		{
			myViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
			OnResize();
		}
		ImGui::Image(DX11::Get().myEditorShaderResourceView.Get(), ImVec2(myViewportSize.x, myViewportSize.y));

		// Gizmos
		if (aSelectedEntity.IsValid())
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();
			ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
		
			Camera* camera = Engine::Get().myRenderer.myMainCamera;
			glm::mat4 projectionMat = camera->GetProjectionMatrix();
			glm::mat4 viewMat = camera->GetViewMatrix();

			auto comp = aSelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = comp->GetTransform();
		
			ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(entityTransform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 newPos;
				glm::vec3 newRot;
				glm::vec3 newScale;
				
				Snow::Math::DecomposeTransform(entityTransform, newPos, newRot, newScale);
				
				glm::vec3 deltaRot = newRot - comp->rotation;

				// Rotation currently broken tbc.

				comp->position = newPos;
				//comp->rotation += deltaRot;
				comp->scale = newScale;
			}
		}
		
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