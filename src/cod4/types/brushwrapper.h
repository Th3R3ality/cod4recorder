#pragma once
#include "cbrushside.h"
#include "cplane.h"

struct BrushWrapper
{
	float mins[3];
	int contents;
	float maxs[3];
	unsigned int numsides;
	cbrushside_t* sides;
	__int16 axialMaterialNum[2][3];
	char* baseAdjacentSide;
	__int16 firstAdjacentSideOffsets[2][3];
	char edgeCount[2][3];
	int totalEdgeCount;
	cplane_t* planes;
};