#pragma once
#include "Engine/Core/Layer.h"
#include <windows.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

namespace Snow
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGui Layer") {};
		~ImGuiLayer() = default;

	private:
		friend class Engine;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
}