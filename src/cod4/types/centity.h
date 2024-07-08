#pragma once
#include "cpose.h"
#include "entitystate.h"
struct centity_t
{
	cpose_t pose;
	LerpEntityState currentState;
	entityState_t nextState;
	bool nextValid;
	bool bMuzzleFlash;
	bool bTrailMade;
	int previousEventSequence;
	int miscTime;
	float lightingOrigin[3];
	void* tree; // XAnimTree_s
};