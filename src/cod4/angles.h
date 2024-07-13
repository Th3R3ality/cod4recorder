#pragma once
#include "../vec.h"
#include "globaldatapointers.h"
#include "../helper.h"
#include "types/usercmd.h"

#define ANGLE2SHORT( x )  ( (int)( ( x ) * 65536 / 360 ) & 0xFFFF )
#define SHORT2ANGLE( x )  ( ( x ) * ( 360.0f / 65536 ) )

float AngleNormalizePI(float angle);
float AngleNormalize360(float angle);
float AngleNormalize180(float angle);
float AngleNormalize90(float angle);
float AngleDelta(float angle1, float angle2);


inline fvec2 AngleNormalize360(fvec2 angles) { return { AngleNormalize360(angles.x), AngleNormalize360(angles.y) }; }
inline fvec2 AngleNormalize180(fvec2 angles) { return { AngleNormalize180(angles.x), AngleNormalize180(angles.y) }; }
inline fvec2 AngleDelta(fvec2 angles1, fvec2 angles2) { return fvec2(AngleDelta(angles1.x, angles2.x), AngleDelta(angles1.y, angles2.y)); }

fvec2 GetViewDeltaToDirection(fvec2 desiredDir);
ivec2 GetViewDeltaToDirection(ivec2 desiredDir);
fvec2 GetRealAngles();
ivec2 GetRealShortAngles();
ivec2 GetRealCmdAngles(const usercmd_t* const cmd);
inline fvec2 GetViewAngles()
{
	return fvec2(dataptr::client->cgameViewangles);
}
void AngleVectors(const fvec3& angles, fvec3* forward, fvec3* right, fvec3* up);


