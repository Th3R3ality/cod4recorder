#include "hk_decl.h"


namespace hooks
{
	void Hook(unsigned long offset, void* detour, void** ppOrig)
	{
		MH_CreateHook((LPVOID)(global::baseAddress + offset), detour, ppOrig);
		MH_EnableHook((LPVOID)(global::baseAddress + offset));
	}

	void Init()
	{
		Hook(funcids::CL_Frame.offset, CL_Frame, &orig_CL_Frame);
		Hook(funcids::CL_CreateCmd.offset, CL_CreateCmd, &orig_CL_CreateCmd);
	}
}