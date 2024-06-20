#include "hk_decl.h"
#include "../../includes.h"
#include "../../imgui/include.h"
#include "../../global.h"
#include "../../userinterface.h"

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(global::window);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false;
HRESULT __stdcall hooks::EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	userinterface::Draw();

	return ((decltype(&EndScene))orig_EndScene)(pDevice);
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ((decltype(&Reset))orig_Reset)(device, pp);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}