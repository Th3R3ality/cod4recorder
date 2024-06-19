#pragma once
#include "hk_decl.h"
//#include "../../imgui/imgui.h"
//#include "../../imgui/imgui_impl_dx9.h"

#include "../../userinterface.h"

bool initialized = false;

void InitImGui(IDirect3DDevice9* device)
{
    //ImGui::CreateContext();
    //ImGui_ImplDX9_Init(device);

    //initialized = true;
}

HRESULT __stdcall hooks::EndScene(IDirect3DDevice9* device)
{
    //if (!initialized)
    //    InitImGui(device);

    //userinterface::Draw();
    //ImGui::GetIO().MouseDrawCursor = false;

    return ((decltype(&EndScene))orig_EndScene)(device);
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp)
{
    //ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = ((decltype(&Reset))orig_Reset)(device, pp);
    //ImGui_ImplDX9_CreateDeviceObjects();
    return result;
}