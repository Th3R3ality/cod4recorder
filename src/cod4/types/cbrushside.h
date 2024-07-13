#pragma once
#include "cplane.h"

#pragma pack(push, 2)
struct cbrushside_t
{
	cplane_t* plane;
	unsigned int materialNum;
	__int16 firstAdjacentSideOffset;
	char edgeCount;
};
#pragma pack(pop)