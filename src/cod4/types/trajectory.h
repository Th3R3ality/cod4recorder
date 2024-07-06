#pragma once
#include "../enums/tr_type.h"
struct trajectory_t
{
	trType_t trType;
	int trTime;
	int trDuration;
	float trBase[3];
	float trDelta[3];
};