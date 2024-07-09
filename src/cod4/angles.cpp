#include "angles.h"

float AngleNormalizePI(float angle)
{
	angle = fmodf(angle + (float)M_PI, 2 * (float)M_PI);
	return angle < 0 ? angle + (float)M_PI : angle - (float)M_PI;
}
float AngleNormalize360(float angle)
{
	return (360.0f / 65536) * ((int)(angle * (65536 / 360.0f)) & 65535);
}
float AngleNormalize180(float angle)
{
	angle = AngleNormalize360(angle);
	if (angle > 180.0)
	{
		angle -= 360.0;
	}
	return angle;
}
float AngleNormalize90(float angle)
{
	return fmodf(angle + 180 + 90, 180) - 90;

}
float AngleDelta(float angle1, float angle2)
{
	return AngleNormalize180(angle1 - angle2);
}

fvec2 GetViewDeltaToDirection(fvec2 desiredDir)
{
	fvec2 deltaangles = fvec2(dataptr::cg->nextSnap->ps.delta_angles);
	fvec2 viewangles = fvec2(dataptr::client->viewangles);
	fvec2 invertedNormViewangles = { -AngleNormalize180(viewangles.x), -AngleNormalize180(viewangles.y) };
	fvec2 delta = AngleDelta(deltaangles, desiredDir);

	return invertedNormViewangles - delta;
}

ivec2 GetViewDeltaToDirection(ivec2 desiredDir)
{
	fvec2 delta = GetViewDeltaToDirection(fvec2(SHORT2ANGLE(desiredDir.x), SHORT2ANGLE(desiredDir.y)));
	return ivec2(ANGLE2SHORT(delta.x), ANGLE2SHORT(delta.y));
}

fvec2 GetRealAngles()
{
	fvec2 deltaangles = fvec2(dataptr::cg->nextSnap->ps.delta_angles);
	fvec2 viewangles = fvec2(dataptr::client->viewangles);
	return AngleNormalize180(viewangles + deltaangles);
}

ivec2 GetRealShortAngles()
{
	const auto angles = GetRealAngles();
	return ivec2(ANGLE2SHORT(angles.x), ANGLE2SHORT(angles.y));
}

ivec2 GetRealCmdAngles(const usercmd_t* const cmd)
{
	fvec2 deltaangles = fvec2(dataptr::cg->nextSnap->ps.delta_angles);
	return ivec2(cmd->viewangles) + ivec2(ANGLE2SHORT(deltaangles.x), ANGLE2SHORT(deltaangles.y));
}
