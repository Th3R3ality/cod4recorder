#pragma once
#include "physgeominfo.h"
#include "physmass.h"

struct PhysGeomList
{
	unsigned int count;
	PhysGeomInfo* geoms;
	PhysMass mass;
};