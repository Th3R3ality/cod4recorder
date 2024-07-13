#include "hk_decl.h"
#include <iostream>
#include "../cod4x.h"

namespace hooks
{
	MH_STATUS HookAddr(void* addr, void* detour, void** ppOrig)
	{
		MH_STATUS status = MH_CreateHook(addr, detour, ppOrig);
		if (status != MH_OK)
			return status;
		status = MH_EnableHook(addr);
		return status;
	}
	MH_STATUS Hook(unsigned long offset, void* detour, void** ppOrig)
	{
		return HookAddr((LPVOID)(global::baseAddress + offset), detour, ppOrig);
	}

	inline void VMTEntryHook(uintptr_t VMT, size_t Index, uintptr_t Destination, uintptr_t* ppOrig = nullptr)
	{
		uintptr_t* Address = (uintptr_t*)(VMT + Index * sizeof(uintptr_t));

		DWORD OldProtection{ 0 };
		VirtualProtect(Address, sizeof(uintptr_t), PAGE_READWRITE, &OldProtection);
		if (ppOrig != nullptr)
			*ppOrig = *Address;
		*Address = Destination;
		VirtualProtect(Address, sizeof(uintptr_t), OldProtection, &OldProtection);
	}

	unsigned long d3dVMT = 0;
	void Init()
	{
		MH_Initialize();
		d3dVMT = **(unsigned long**)(global::baseAddress + dataid::d3d::device.offset);

		//printf("hooking CL_Frame : %s\n", MH_StatusToString(Hook(funcids::CL_Frame.offset, CL_Frame, &orig_CL_Frame)));
		printf("hooking CL_CreateCmd : %s\n",
			MH_StatusToString(
				Hook(dataid::func::CL_CreateCmd.offset, CL_CreateCmd, &orig_CL_CreateCmd)));

		printf("hooking simcrash1 : %s\n",
			MH_StatusToString(
				HookAddr((void*)dataid::func::simcrash1.addr, simcrash1, &orig_simcrash1)));

		printf("hooking simcrash2 : %s\n",
			MH_StatusToString(
				HookAddr((void*)dataid::func::simcrash2.addr, simcrash2, &orig_simcrash2)));

		VMTEntryHook(d3dVMT, d3dindex::end_scene, (uintptr_t)EndScene, &orig_EndScene);
		printf("hooking EndScene : %x\n", orig_EndScene);

		VMTEntryHook(d3dVMT, d3dindex::reset, (uintptr_t)Reset, &orig_Reset);
		printf("hooking Reset : %x\n", orig_Reset);


		if (uintptr_t base = cod4x::base())
		{
			printf("hooking antihook {cod4x} : %s\n",
				MH_StatusToString(
					HookAddr((void*)(base + 0x43580), antihook, &orig_antihook)));
			
		}
	}
	void Detach()
	{
		VMTEntryHook(d3dVMT, d3dindex::end_scene, orig_EndScene);
		VMTEntryHook(d3dVMT, d3dindex::reset, orig_Reset);

		MH_DisableHook(MH_ALL_HOOKS);

		MH_Uninitialize();
	}
}