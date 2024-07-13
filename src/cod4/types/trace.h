#pragma once
#include "../enums/tracehittype.h"

struct __declspec(align(4)) trace_t
{
	float fraction;
	float normal[3];
	int surfaceFlags;
	int contents;
	const char* material;
	TraceHitType hitType;
	unsigned __int16 hitId;
	unsigned __int16 modelIndex;
	unsigned __int16 partName;
	unsigned __int16 partGroup;
	bool allsolid;
	bool startsolid;
	bool walkable;
};