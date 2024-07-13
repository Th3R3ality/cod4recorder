#pragma once
#include "fxtrailvertex.h"
struct FxTrailDef
{
	int scrollTimeMsec;
	int repeatDist;
	int splitDist;
	int vertCount;
	FxTrailVertex* verts;
	int indCount;
	unsigned __int16* inds;
};