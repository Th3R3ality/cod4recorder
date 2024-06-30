#pragma once
#include "../../includes.h"
#include "../../minhook/MinHook.h"

#include "../../global.h"

#include "../types/include.h"
#include "../dataid.h"

// cuck macro
#define PUSHALL \
__asm pushad

//__asm push eax \
//__asm push ebx \
//__asm push ecx \
//__asm push edx \
//__asm push esi \
//__asm push edi

// cuck macro part 2
#define POPALL \
__asm popad
//__asm pop edi \
//__asm pop esi \
//__asm pop edx \
//__asm pop ecx \
//__asm pop ebx \
//__asm pop eax 


namespace hooks
{
	namespace d3dindex
	{
		constexpr auto reset = 16;
		constexpr auto end_scene = 42;
	}

	inline void* orig_CL_Frame = nullptr;
	void CL_Frame(int localClientNum);

	inline void* orig_CL_CreateCmd = nullptr;
	usercmd_t* CL_CreateCmd(usercmd_t* cmd, int localClientNum);

	inline uintptr_t orig_EndScene = 0;
	HRESULT __stdcall EndScene(LPDIRECT3DDEVICE9 pDevice);

	inline uintptr_t orig_Reset = 0;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);

	void Init();
	void Detach();
}