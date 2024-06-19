#include "hk_decl.h"
#include <iostream>

namespace hooks
{
	MH_STATUS Hook(unsigned long offset, void* detour, void** ppOrig)
	{
		MH_STATUS status = MH_CreateHook((LPVOID)(global::baseAddress + offset), detour, ppOrig);
		if (status != MH_OK)
			return status;
		status = MH_EnableHook((LPVOID)(global::baseAddress + offset));
		return status;
	}

	void Init()
	{
		//printf("hooking CL_Frame : %s\n", MH_StatusToString(Hook(funcids::CL_Frame.offset, CL_Frame, &orig_CL_Frame)));
		printf("hooking CL_CreateCmd : %s\n", MH_StatusToString(Hook(funcids::CL_CreateCmd.offset, CL_CreateCmd, &orig_CL_CreateCmd)));
	}
}