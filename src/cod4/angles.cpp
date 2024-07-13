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

void AngleVectors(const fvec3& angles, fvec3* forward, fvec3* right, fvec3* up)
{
	float angle;
	static float sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);

	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}
	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}
