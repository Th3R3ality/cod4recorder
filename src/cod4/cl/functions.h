#pragma once
#include "../globaldatapointers.h"
#include "../types/usercmd.h"

inline usercmd_t* CL_GetUserCmd(const int cmdNumber)
{
	return &dataptr::client->cmds[cmdNumber & 0x7F];
}