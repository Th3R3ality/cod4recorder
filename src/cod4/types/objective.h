#pragma once
#include "../enums/objectivestate.h"

struct Objective_t
{
	ObjectiveState_t state;
	float origin[3];
	int entNum;
	int teamNum;
	int icon;
};