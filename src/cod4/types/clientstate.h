#pragma once
#include "../enums/team.h"
struct clientState_t
{
	int clientIndex;
	team_t team;
	int modelindex;
	int attachModelIndex[6];
	int attachTagIndex[6];
	char name[16]; //stolen by cod4x (DO NOT USE)
	float maxSprintTimeMultiplier;
	int rank;
	int prestige;
	int perks;
	int attachedVehEntNum;
	int attachedVehSlotIndex;
};