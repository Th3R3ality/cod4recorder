#pragma once
#include <optional>
#define _USE_MATH_DEFINES
#include <math.h>
#include "cod4/include.h"
#include "global.h"
#include "cod4/recorder.h"
#include "cod4/replayer.h"
#include "vec.h"



inline dvar_t* GetDvar(unsigned long DvarOffset)
{
	if (global::baseAddress == 0)
		return nullptr;
	return *(dvar_t**)(global::baseAddress + DvarOffset);
}

inline float Magnitude(const float vec[3])
{
	return std::sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}
inline float MagnitudeXY(const float vec[3])
{
	return std::sqrtf(vec[0] * vec[0] + vec[1] * vec[1]);
}

inline std::optional<ivec2> WorldToScreen(const fvec3& location)
{
	const refdef_t* refdef = &dataptr::cg->refdef;

	const int centerX = refdef->width / 2;
	const int centerY = refdef->height / 2;

	const fvec3 vright = refdef->viewaxis[1];
	const fvec3 vup = refdef->viewaxis[2];
	const fvec3 vfwd = refdef->viewaxis[0];

	const fvec3 vLocal = location - refdef->vieworg;
	fvec3 vTransform;

	vTransform.x = vLocal.dot(vright);
	vTransform.y = vLocal.dot(vup);
	vTransform.z = vLocal.dot(vfwd);

	if (vTransform.z < 0.01)
	{
		return std::nullopt;
	}
	ivec2 out;

	out.x = static_cast<int>(centerX * (1 - (vTransform.x / refdef->tanHalfFovX / vTransform.z)));
	out.y = static_cast<int>(centerY * (1 - (vTransform.y / refdef->tanHalfFovY / vTransform.z)));


	if (vTransform.z > 0)
		return out;

	return std::nullopt;
}
