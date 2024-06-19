#include "hk_decl.h"
#include <iostream>

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

	unsigned long d3dVMT = 0;
	void Init()
	{
		MH_Initialize();

		//printf("hooking CL_Frame : %s\n", MH_StatusToString(Hook(funcids::CL_Frame.offset, CL_Frame, &orig_CL_Frame)));
		printf("hooking CL_CreateCmd : %s\n",
			MH_StatusToString(
				Hook(funcids::CL_CreateCmd.offset, CL_CreateCmd, &orig_CL_CreateCmd)));

		d3dVMT = **(unsigned long**)(global::baseAddress + d3dids::device.offset);

		printf("hooking EndScene : %s\n",
			MH_StatusToString(
				HookAddr((void*)(d3dVMT + d3dindex::end_scene * sizeof(void*)), EndScene, &orig_EndScene)));

		printf("hooking Reset : %s\n",
			MH_StatusToString(
				HookAddr((void*)(d3dVMT + d3dindex::reset * sizeof(void*)), Reset, &orig_Reset)));
	}
	void Detach()
	{
		MH_DisableHook(MH_ALL_HOOKS);

		MH_Uninitialize();
	}
}