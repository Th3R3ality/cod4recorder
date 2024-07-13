#pragma once
#include "../types/pmove.h"
#include "../types/pml.h"
#include "../types/playerstate.h"

void PM_UpdateViewAngles(playerState_t* ps, int msec, usercmd_t* cmd, char handler);
void PM_UpdatePlayerWalkingFlag(pmove_t* pm);
void PM_CheckDuck(pmove_t* pm, pml_t* pml);
void PM_Mantle_Check(pmove_t* pm, pml_t* pml);
void PM_Mantle_Move(playerState_t* ps, pmove_t* pm, pml_t* pml);
void PM_Weapon(pmove_t* pm, pml_t* pml);
void PM_UpdatePronePitch(pml_t* pml, pmove_t* pm);
void PM_DropTimers(playerState_t* ps, pml_t* pml);
void PM_Footsteps(pmove_t* pm, pml_t* pml);
void PM_OverBounce(pmove_t* pm, pml_t* pml);

inline void __cdecl PM_GroundTrace(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_GroundTrace))0x00410660;
	func(pm, pml);
}
inline void __cdecl PM_LadderMove(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_LadderMove))0x41DD0;
	func(pm, pml);
}
inline void __cdecl PM_WalkMove(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_WalkMove))0x40F7A0;
	func(pm, pml);
}
inline void __cdecl PM_AirMove(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_AirMove))0x40F680;
	func(pm, pml);
}
inline void __cdecl PM_CheckLadderMove(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_CheckLadderMove))0x413A60;
	func(pm, pml);
}
inline void __cdecl PM_AdjustAimSpreadScale(pmove_t* pm, pml_t* pml)
{
	static auto func = (decltype(&PM_AdjustAimSpreadScale))0x418870;
	func(pm, pml);
}
inline void PM_UpdateSprint(pmove_t* pm)
{
	static auto func = (decltype(&PM_UpdateSprint))0x40E6A0;
	func(pm);
}

void Sys_SnapVector(float* v);

pmove_t PM_Create(playerState_t* ps, usercmd_t* cmd, usercmd_t* oldcmd);
pml_t PML_Create(pmove_t* pm, int FPS);