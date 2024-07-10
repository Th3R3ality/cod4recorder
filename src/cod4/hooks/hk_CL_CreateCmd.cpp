#include "hk_decl.h"
#include <iostream>

#include "../recorder.h"
#include "../replayer.h"
#include "../dataid.h"
#include "../../helper.h"


__declspec(noinline) void __fastcall logic(usercmd_t* const cmd, [[maybe_unused]] int localClientNum )
{
	if (global::debugPrints)
		printf("CL_CreateCmd\n");

	//playerState_t* ps = &dataptr::cg->predictedPlayerState;
	//printf("pm_type: %i\n", ps->pm_type);

	if (recorder::isRecording)
	{
		recorder::CaptureCommand(cmd);
	}
	else if (replayer::isReplaying)
	{
		replayer::ModifyNextCommand(cmd);
	}

	if (global::viewcmd)
	{
		memcpy(&global::cmd, cmd, sizeof(usercmd_t));
	}
}

__declspec(naked) usercmd_t* hooks::CL_CreateCmd(usercmd_t* cmd, int localClientNum)
{
	__asm {
		call orig_CL_CreateCmd

		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE

		mov cmd, eax
		mov localClientNum, esi
		
		PUSHALL
	}
	
	logic(cmd, localClientNum);

	__asm {
		POPALL
		

		mov esp, ebp
		pop ebp

		ret
	}
}