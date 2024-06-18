#include "hk_decl.h"
#include <iostream>

#include "../recorder.h"
#include "../replayer.h"
#include "../dataid.h"
#include "../helper.h"


int startServertime = 0;
int viewangle0offset = 0;
int viewangle1offset = 0;
unsigned short g_lastFps = 0;
__declspec(noinline) void __fastcall logic(usercmd_t* cmd, int localClientNum)
{
	if (global::debugPrints)
		printf("CL_CreateCmd\n");
	static dvar_t* dvar_maxfps = GetDvar(dvarids::com_maxfps.offset);

	global::createcmdcount++;

	if (recorder::isRecording)
	{
		if (recorder::recordedCmds == 0)
		{
			startServertime = cmd->servertime;
			viewangle0offset = cmd->viewangles[0];
			viewangle1offset = cmd->viewangles[1];
		}

		recorder::Smallcmd scmd;
		scmd.servertime = cmd->servertime - startServertime;
		scmd.buttons = cmd->buttons;
		scmd.viewangles[0] = cmd->viewangles[0];
		scmd.viewangles[1] = cmd->viewangles[1];
		scmd.forwardmove = cmd->forwardmove;
		scmd.sidemove = cmd->sidemove;
		scmd.nextFps = 0;
		recorder::recording.push_back(scmd);

		if (recorder::recording.size() > 1)
		{
			unsigned short currentFps = dvar_maxfps->current.integer;
			if (currentFps != g_lastFps)
			{
				recorder::recording.at(recorder::recording.size() - 2).nextFps = currentFps;
			}

			g_lastFps = dvar_maxfps->current.integer;
		}

		recorder::recordedCmds++;
	}


	if (replayer::isReplaying)
	{
		if (replayer::replayIndex >= recorder::recording.size())
		{
			replayer::replayIndex = 0;
			replayer::isReplaying = false;
			return;
		}
		auto& smallcmd = recorder::recording.at(replayer::replayIndex);

		if (replayer::replayIndex == 0)
		{
			startServertime = cmd->servertime;
			viewangle0offset = recorder::recording.at(0).viewangles[0] - cmd->viewangles[0];
			viewangle1offset = recorder::recording.at(0).viewangles[1] - cmd->viewangles[1];
		}


		if (smallcmd.nextFps != 0)
			dvar_maxfps->current.integer = smallcmd.nextFps;

		cmd->servertime = startServertime + smallcmd.servertime;
		cmd->buttons = smallcmd.buttons;
		cmd->viewangles[0] = smallcmd.viewangles[0] - viewangle0offset;
		//if (cmd->viewangles[0] < 0)			cmd->viewangles[0] += 0xFFFF;
		//if (cmd->viewangles[0] > 0xFFFF)	cmd->viewangles[0] -= 0xFFFF;

		cmd->viewangles[1] = smallcmd.viewangles[1] - viewangle1offset;
		//if (cmd->viewangles[1] < 0)			cmd->viewangles[1] += 0xFFFF;
		//if (cmd->viewangles[1] > 0xFFFF)	cmd->viewangles[1] -= 0xFFFF;

		cmd->forwardmove = smallcmd.forwardmove;
		cmd->sidemove = smallcmd.sidemove;

		replayer::replayIndex++;
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

	{
		logic(cmd, localClientNum);
	}

	__asm {
		POPALL

		mov esp, ebp
		pop ebp

		ret
	}
}