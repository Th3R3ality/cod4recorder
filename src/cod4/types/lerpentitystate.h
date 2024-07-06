#pragma once
#include "trajectory.h"
struct LerpEntityStateTurret
{
	float gunAngles[3];
};
struct LerpEntityStateLoopFx
{
	float cullDist;
	int period;
};
struct LerpEntityStatePrimaryLight
{
	char colorAndExp[4];
	float intensity;
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
};
struct LerpEntityStatePlayer
{
	float leanf;
	int movementDir;
};
struct LerpEntityStateVehicle
{
	float bodyPitch;
	float bodyRoll;
	float steerYaw;
	int materialTime;
	float gunPitch;
	float gunYaw;
	int teamAndOwnerIndex;
};
struct LerpEntityStateMissile
{
	int launchTime;
};
struct LerpEntityStateSoundBlend
{
	float lerp;
};
struct LerpEntityStateBulletHit
{
	float start[3];
};
struct LerpEntityStateEarthquake
{
	float scale;
	float radius;
	int duration;
};
struct LerpEntityStateCustomExplode
{
	int startTime;
};
struct LerpEntityStateExplosion
{
	float innerRadius;
	float magnitude;
};
struct LerpEntityStateExplosionJolt
{
	float innerRadius;
	float impulse[3];
};
struct LerpEntityStatePhysicsJitter
{
	float innerRadius;
	float minDisplacement;
	float maxDisplacement;
};
struct LerpEntityStateAnonymous
{
	int data[7];
};

union LerpEntityStateTypeUnion
{
	LerpEntityStateTurret turret;
	LerpEntityStateLoopFx loopFx;
	LerpEntityStatePrimaryLight primaryLight;
	LerpEntityStatePlayer player;
	LerpEntityStateVehicle vehicle;
	LerpEntityStateMissile missile;
	LerpEntityStateSoundBlend soundBlend;
	LerpEntityStateBulletHit bulletHit;
	LerpEntityStateEarthquake earthquake;
	LerpEntityStateCustomExplode customExplode;
	LerpEntityStateExplosion explosion;
	LerpEntityStateExplosionJolt explosionJolt;
	LerpEntityStatePhysicsJitter physicsJitter;
	LerpEntityStateAnonymous anonymous;
};

struct LerpEntityState
{
	int eFlags;
	trajectory_t pos;// for calculating position
	trajectory_t apos;// for calculating angles
	LerpEntityStateTypeUnion u;
};