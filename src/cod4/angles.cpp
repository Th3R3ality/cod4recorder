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
	fvec2 deltaangles = { dataptr::cg->nextSnap->ps.delta_angles[0], dataptr::cg->nextSnap->ps.delta_angles[1] };
	fvec2 viewangles = { dataptr::client->viewangles[0],dataptr::client->viewangles[1] };
	fvec2 invertedNormViewangles = { -AngleNormalize180(viewangles.x), -AngleNormalize180(viewangles.y) };
	fvec2 delta = AngleDelta(deltaangles, desiredDir);
	return invertedNormViewangles - delta;
}

fvec2 GetRealAngles()
{
	fvec2 deltaangles = { dataptr::cg->nextSnap->ps.delta_angles[0], dataptr::cg->nextSnap->ps.delta_angles[1] };
	fvec2 viewangles = { dataptr::client->viewangles[0], dataptr::client->viewangles[1] };
	return AngleNormalize180(viewangles + deltaangles);
}