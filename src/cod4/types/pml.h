#pragma once
#include "trace.h"
#include "../../vec.h"

struct pml_t
{
	fvec3 forward;
	fvec3 right;
	fvec3 up;
	float frametime;
	int msec;
	int walking;
	int groundPlane;
	int almostGroundPlane;
	trace_t groundTrace;
	float impactSpeed;
	fvec3 previous_origin;
	fvec3 previous_velocity;
};