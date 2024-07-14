#include "simulation.h"
#include "globaldatapointers.h"
#include "helper.h"
#include "angles.h"

#include "types/dvar.h"

#include "enums/pmflags.h"
#include "enums/weaponflags.h"
#include "enums/pmtype.h"
#include "enums/pmeflags.h"

#include "pm/functions.h"
#include "cl/functions.h"

constexpr int _fps = 33;

void simulation::SimulateStep(pmove_t* pm, pml_t* pml)
{
	static dvar_t* mantle_enable = Dvar_Find("mantle_enable");
	playerState_t* ps = pm->ps;

	simulating = true;

	if (!simulating)
		return;

	pm->cmd.forwardmove = 127; // controller->forwardmove;
	pm->cmd.sidemove = 0; // controller->rightmove;
	pm->cmd.buttons = 0; // controller->buttons;

	fvec3 deltas = {};

	// DO ROTATION DELTA LOGIC HERE

	ps->viewangles += deltas;
	pm->cmd.viewangles += Angles2Shorts(deltas);



	if ((ps->pm_flags & PMF_MELEE_CHARGE) != 0)
		pm->cmd.forwardmove = 127;

	if ((ps->pm_flags & PMF_FROZEN) != 0)
	{
		pm->cmd.buttons &= 0x1300u;
	}

	if ((ps->pm_flags & PMF_RESPAWNED) != 0)
	{
		pm->cmd.buttons &= 0x1301u;
	}

	if (ps->weaponstate == WEAPON_FIRING)
	{
		int weaponIdx = ps->weapon;
		if (weaponIdx)
		{
			if (dataptr::BG_WeaponDefs[weaponIdx]->freezeMovementWhenFiring)
			{
				pm->cmd.buttons &= 0xFFFFFB3F;
				pm->cmd.forwardmove = 0;
				pm->cmd.sidemove = 0;
				ps->velocity[0] = 0.0;
				ps->velocity[1] = 0.0;
				ps->velocity[2] = 0.0;
			}
		}
	}

	ps->pm_flags &= (~PMF_PRONE_BLOCKED);
	if (ps->pm_type >= PM_DEAD)
		pm->tracemask &= 0xFDFF3FFF;

	if ((ps->pm_flags & PMF_PRONE) == 0 ||
		dataptr::BG_WeaponDefs[ps->weapon]->overlayReticle &&
		ps->fWeaponPosFrac > 0.0)
	{
		ps->pm_flags &= (~PMF_PRONEMOVE_OVERRIDDEN);
	}

	if ((ps->pm_flags & PMF_SIGHT_AIMING) != 0 && ps->viewHeightTarget == 11)
	{
		if (dataptr::BG_WeaponDefs[ps->weapon]->overlayReticle)
		{
			if (0.0f >= ps->fWeaponPosFrac)
			{
				pm->cmd.forwardmove = 0;
				pm->cmd.sidemove = 0;
			}
		}
		else
		{
			pm->cmd.forwardmove = 0;
			pm->cmd.sidemove = 0;
		}
	}
	ps->eFlags &= (~EF_FIRING);

	if ((ps->pm_flags & PMF_RESPAWNED) == 0)
	{
		if ((!ps->weaponstate || ps->weaponstate == 5) &&
			ps->ammoclip[dataptr::BG_WeaponDefs[ps->weapon]->iClipIndex + 4] &&
			(pm->cmd.buttons & 1) != 0)
		{
			ps->eFlags |= EF_FIRING;
		}
	}
	if (ps->pm_type < PM_DEAD && (pm->cmd.buttons & 0x101) == 0)
		ps->pm_flags &= (~PMF_RESPAWNED);

	pml->previous_origin[0] = ps->origin[0];
	pml->previous_origin[1] = ps->origin[1];
	pml->previous_origin[2] = ps->origin[2];
	pml->previous_velocity[0] = ps->velocity[0];
	pml->previous_velocity[1] = ps->velocity[1];
	pml->previous_velocity[2] = ps->velocity[2];


	pml->msec = 1000 / _fps; //simulate fps

	pml->frametime = (float)pml->msec / 1000.f;
	pm->cmd.servertime += pml->msec;
	ps->commandTime += pml->msec;

	PM_AdjustAimSpreadScale(pm, pml);
	PM_UpdateViewAngles(ps, pml->msec, &pm->cmd, pm->handler);

	AngleVectors(ps->viewangles, &pml->forward, &pml->right, &pml->up); //set viewangles

	char fwd = pm->cmd.forwardmove;
	if (fwd >= 0)
	{
		if (fwd > 0 || pm->cmd.sidemove)
			ps->pm_flags &= ~PMF_BACKWARDS_RUN;
	}
	else
	{
		ps->pm_flags |= PMF_BACKWARDS_RUN;
	}

	if ((ps->pm_flags & PMF_MANTLE) == 0)
	{
		PM_UpdateSprint(pm);
		PM_UpdatePlayerWalkingFlag(pm);
		PM_CheckDuck(pm, pml);
		PM_GroundTrace(pm, pml);
	}

	PM_Mantle_Check(pm, pml);

	if ((ps->pm_flags & PMF_MANTLE) != 0)
	{
		if ((ps->pm_flags & PMF_LADDER) != 0)
			ps->pm_flags = ps->pm_flags & (~PMF_LADDER) | PMF_LADDER_FALL;
		ps->groundEntityNum = 1023;
		pml->groundPlane = 0;
		pml->walking = 0;

		PM_UpdateSprint(pm);
		PM_UpdatePlayerWalkingFlag(pm);
		PM_CheckDuck(pm, pml);
		if (mantle_enable->current.enabled)
			PM_Mantle_Move(ps, pm, pml);

		PM_Weapon(pm, pml);
	}
	else
	{
		PM_UpdatePronePitch(pml, pm);
		PM_DropTimers(ps, pml);
		PM_CheckLadderMove(pm, pml);

		if ((ps->pm_flags & PMF_LADDER) != 0)
		{
			PM_LadderMove(pm, pml);
		}
		else if (pml->walking)
		{
			PM_WalkMove(pm, pml);
		}
		else
		{
			PM_AirMove(pm, pml);
		}
		PM_GroundTrace(pm, pml); //call groundtrace after 
		PM_Weapon(pm, pml);
		PM_OverBounce(pm, pml);
		Sys_SnapVector(ps->velocity); //Sys_SnapVector | not called in singleplayer
	}

	if (pm->cmd.forwardmove > 0)
		pm->cmd.forwardmove = 127;
	else if (pm->cmd.forwardmove < 0)
		pm->cmd.forwardmove = -127;

	if (pm->cmd.sidemove > 0)
		pm->cmd.sidemove = 127;
	else if (pm->cmd.sidemove < 0)
		pm->cmd.sidemove = -127;

	memcpy(&pm->oldcmd, &pm->cmd, sizeof(pm->oldcmd));

	simulating = false;
	return;
}

void simulation::test::ForwardSteps(int steps)
{
	pmove_t pm = PM_Create(&dataptr::cg->predictedPlayerState, CL_GetUserCmd(dataptr::client->cmdNumber), CL_GetUserCmd(dataptr::client->cmdNumber - 1));
	pml_t pml = PML_Create(&pm, Dvar_Find("com_maxfps")->current.integer);
	playerState_t* ps = pm.ps;

	printf("simulating %i steps forward\n", steps);
	printf("starting @ {%f, %f, %f}\n", ps->origin.x, ps->origin.y, ps->origin.z);

	trail.clear();
	trail.emplace_back(ps->origin);

	for (int i = 0; i < steps; i++)
	{
		SimulateStep(&pm, &pml);
		//printf("step %i @ {%f, %f, %f}\n", i, ps->origin.x, ps->origin.y, ps->origin.z);
		trail.emplace_back(ps->origin);
	}
	printf("simulation done\n");
}
