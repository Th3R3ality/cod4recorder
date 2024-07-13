#pragma once
#include "brushwrapper.h"

struct PhysGeomInfo
{
	BrushWrapper* brush;
	int type;
	float orientation[3][3];
	float offset[3];
	float halfLengths[3];
};