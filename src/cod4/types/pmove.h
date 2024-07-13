#pragma once
#include "playerstate.h"
#include "usercmd.h"

struct pmove_t
{
	// state (in / out)
	playerState_t* ps;
	usercmd_t cmd;
	usercmd_t oldcmd;
	int tracemask; // collide against these types of surfaces
	int numtouch;
	int touchents[32];
	float mins[3];
	float maxs[3];
	float xyspeed;
	int proneChange;
	float maxSprintTimeMultiplier;
	bool mantleStarted;
	float mantleEndPos[3];
	int mantleDuration;
	int viewChangeTime;
	float viewChange;
	char handler;
};