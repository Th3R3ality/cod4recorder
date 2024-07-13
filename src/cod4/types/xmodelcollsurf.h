#pragma once
#include "xmodelcolltri.h"

struct XModelCollSurf_t
{
	XModelCollTri_t* collTris;
	int numCollTris;
	float mins[3];
	float maxs[3];
	int boneIdx;
	int contents;
	int surfFlags;
};