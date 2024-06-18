#include "hk_decl.h"
#include <iostream>

__declspec(noinline) void __fastcall logic(int localClientNum)
{
	if (global::debugPrints)
		printf("CL_Frame\n");
}

__declspec(naked) void hooks::CL_Frame(int localClientNum)
{
	__asm {

		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE

		mov localClientNum, eax

		PUSHALL
	}

	{
		logic(localClientNum);
	}

	__asm {
		POPALL

		mov esp, ebp
		pop ebp

		call orig_CL_Frame

		ret
	}
}